#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

PASSED=0
FAILED=0
TOTAL=0
TEMP_DIR="/tmp/minishell_leak_test"
VALGRIND_LOG="$TEMP_DIR/valgrind.log"

mkdir -p "$TEMP_DIR"


VALGRIND_OPTS="--leak-check=full --show-leak-kinds=definite --track-fds=yes --errors-for-leak-kinds=definite --error-exitcode=42 --trace-children=no"

if [ -z "$DEBUGINFOD_URLS" ]; then
    export DEBUGINFOD_URLS="https://debuginfod.archlinux.org"
fi

cleanup() {
    rm -rf "$TEMP_DIR"
}

trap cleanup EXIT

check_leak() {
    local name="$1"
    local commands="$2"
    
    TOTAL=$((TOTAL + 1))
    
    echo -e "\n${BLUE}${BOLD}Testing:${RESET} $name"
    echo -e "${YELLOW}Commands:${RESET} $(echo "$commands" | tr '\n' ';' | sed 's/;$//')"
    
    echo -e "$commands" | valgrind $VALGRIND_OPTS ./minishell > "$VALGRIND_LOG" 2>&1
    EXIT_CODE=$?
    
    DEFINITELY_LOST=$(grep "definitely lost:" "$VALGRIND_LOG" | head -1 | awk '{print $4}')
    INDIRECTLY_LOST=$(grep "indirectly lost:" "$VALGRIND_LOG" | head -1 | awk '{print $4}')
    
    MEMORY_ERRORS=$(grep -E "Invalid (read|write)|Conditional jump|Use of uninitialised" "$VALGRIND_LOG" | wc -l)
    
    # Parse leak information - only fail if there are actual memory leks (not just FD warnings)
    if [ "$DEFINITELY_LOST" = "0" ] && [ "$INDIRECTLY_LOST" = "0" ] && [ $MEMORY_ERRORS -eq 0 ]; then
        echo -e "${GREEN}✓ PASS${RESET} - No memory leaks detected"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗ FAIL${RESET} - Memory leak detected!"
        echo -e "${YELLOW}Definitely lost: ${DEFINITELY_LOST} bytes${RESET}"
        echo -e "${YELLOW}Indirectly lost: ${INDIRECTLY_LOST} bytes${RESET}"
        echo -e "${YELLOW}Memory errors: ${MEMORY_ERRORS}${RESET}"
   

        if [ "$DEFINITELY_LOST" != "0" ] || [ "$INDIRECTLY_LOST" != "0" ]; then
            echo -e "\n${YELLOW}Leak Details:${RESET}"
            grep -A 10 "definitely lost" "$VALGRIND_LOG" | head -20
        fi
        
   
        if [ $MEMORY_ERRORS -gt 0 ]; then
            echo -e "\n${YELLOW}Memory Errors:${RESET}"
            grep -B 2 -A 5 -E "Invalid (read|write)|Conditional jump|Use of uninitialised" "$VALGRIND_LOG" | head -30
        fi
        
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo -e "${BOLD}${BLUE}================================================${RESET}"
echo -e "${BOLD}    MINISHELL MEMORY LEAK TEST SUITE${RESET}"
echo -e "${BOLD}${BLUE}================================================${RESET}"


if [ ! -f ./minishell ]; then
    echo -e "${RED}Error: ./minishell not found${RESET}"
    echo "Please compile minishell first: make"
    exit 1
fi

# Check if valgrind is installed
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}Error: valgrind not found${RESET}"
    echo "Please install valgrind: sudo pacman -S valgrind"
    exit 1
fi

echo -e "\n${BOLD}=== SIMPLE COMMANDS ===${RESET}"
check_leak "Empty input and exit" "exit"
check_leak "Single echo" "echo hello\nexit"
check_leak "Multiple echoes" "echo hello\necho world\necho test\nexit"
check_leak "pwd command" "pwd\nexit"

echo -e "\n${BOLD}=== BUILTINS ===${RESET}"
check_leak "cd to home" "cd ~\npwd\nexit"
check_leak "cd to tmp and back" "cd /tmp\ncd -\nexit"
check_leak "cd with invalid path" "cd /nonexistent\nexit"
check_leak "export variable" "export TEST=value\nenv | grep TEST\nexit"
check_leak "export multiple" "export A=1\nexport B=2\nexport C=3\nenv\nexit"
check_leak "unset variable" "export TEST=value\nunset TEST\nexit"
check_leak "unset multiple" "export A=1\nexport B=2\nunset A\nunset B\nexit"

echo -e "\n${BOLD}=== EXTERNAL COMMANDS ===${RESET}"
check_leak "ls command" "ls\nexit"
check_leak "ls with options" "ls -la\nexit"
check_leak "cat file" "cat Makefile\nexit"
check_leak "grep command" "grep minishell README.md\nexit"
check_leak "Multiple external commands" "ls\npwd\ncat Makefile\nexit"

echo -e "\n${BOLD}=== ENVIRONMENT VARIABLES ===${RESET}"
check_leak "Variable expansion" "export VAR=hello\necho \$VAR\nexit"
check_leak "Multiple expansions" "export A=1\nexport B=2\necho \$A \$B\nexit"
check_leak "PATH variable" "echo \$PATH\nexit"
check_leak "HOME variable" "echo \$HOME\nexit"
check_leak "Exit status" "ls\necho \$?\nexit"

echo -e "\n${BOLD}=== QUOTES ===${RESET}"
check_leak "Double quotes" "echo \"hello world\"\nexit"
check_leak "Single quotes" "echo 'hello world'\nexit"
check_leak "Mixed quotes" "echo \"hello\" 'world'\nexit"
check_leak "Quotes with variables" "export VAR=test\necho \"\$VAR\"\nexit"

echo -e "\n${BOLD}=== REDIRECTIONS ===${RESET}"
check_leak "Output redirect" "echo test > $TEMP_DIR/out.txt\nexit"
check_leak "Append redirect" "echo first > $TEMP_DIR/out.txt\necho second >> $TEMP_DIR/out.txt\nexit"
check_leak "Input redirect" "echo content > $TEMP_DIR/in.txt\ncat < $TEMP_DIR/in.txt\nexit"
check_leak "Multiple redirects" "echo test > $TEMP_DIR/out1.txt\necho test2 > $TEMP_DIR/out2.txt\nexit"

echo -e "\n${BOLD}=== PIPES ===${RESET}"
check_leak "Simple pipe" "echo hello | cat\nexit"
check_leak "ls pipe grep" "ls | grep Makefile\nexit"
check_leak "Multiple pipes" "echo test | cat | cat\nexit"
check_leak "cat pipe grep" "cat Makefile | grep CC\nexit"

echo -e "\n${BOLD}=== PIPES + REDIRECTIONS ===${RESET}"
check_leak "Pipe with output redirect" "echo test | cat > $TEMP_DIR/pipe.txt\nexit"
check_leak "Pipe with input redirect" "echo content > $TEMP_DIR/in.txt\ncat < $TEMP_DIR/in.txt | grep content\nexit"
check_leak "Complex pipe redirect" "ls | grep Make | cat > $TEMP_DIR/result.txt\nexit"

echo -e "\n${BOLD}=== HEREDOC ===${RESET}"
check_leak "Simple heredoc" "cat << EOF\nHello\nWorld\nEOF\nexit"
check_leak "Heredoc with variables" "export VAR=test\ncat << EOF\n\$VAR\nEOF\nexit"

echo -e "\n${BOLD}=== ERROR CASES ===${RESET}"
check_leak "Command not found" "nonexistentcommand\nexit"
check_leak "Invalid option" "ls --invalidoption\nexit"
check_leak "Permission denied" "cd /root\nexit"
check_leak "File not found" "cat /nonexistent/file\nexit"

echo -e "\n${BOLD}=== STRESS TESTS ===${RESET}"
check_leak "Many commands" "echo 1\necho 2\necho 3\necho 4\necho 5\necho 6\necho 7\necho 8\necho 9\necho 10\nexit"
check_leak "Many exports" "export A=1\nexport B=2\nexport C=3\nexport D=4\nexport E=5\nexit"
check_leak "Long command" "echo this is a very long command with many arguments and words to test memory allocation\nexit"
check_leak "Multiple pipes" "echo test | cat | cat | cat | cat\nexit"

echo -e "\n${BOLD}=== EXIT CODES ===${RESET}"
check_leak "Exit with code 0" "exit 0"
check_leak "Exit with code 1" "exit 1"
check_leak "Exit with code 42" "exit 42"

# Summary
echo -e "\n${BOLD}${BLUE}================================================${RESET}"
echo -e "${BOLD}MEMORY LEAK TEST SUMMARY${RESET}"
echo -e "${BOLD}${BLUE}================================================${RESET}"
echo -e "${GREEN}Passed:  $PASSED/$TOTAL${RESET}"
echo -e "${RED}Failed:  $FAILED/$TOTAL${RESET}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}${BOLD}✓ ALL TESTS PASSED - NO MEMORY LEAKS! ✓${RESET}\n"
    echo -e "${GREEN}Your minishell is leak-free! 🎉${RESET}"
    exit 0
else
    echo -e "\n${RED}${BOLD}✗ SOME TESTS FAILED - MEMORY LEAKS DETECTED ✗${RESET}\n"
    echo -e "${YELLOW}Check the output above for details on which tests failed.${RESET}"
    echo -e "${YELLOW}Full valgrind log available at: $VALGRIND_LOG${RESET}"
    exit 1
fi
