#!/bin/bash
# Quick test script for Minishell
# Tests basic functionality

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

PASSED=0
FAILED=0
TOTAL=0

test_command() {
    local name="$1"
    local command="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    echo -e "${BLUE}Testing:${RESET} $name"
    
    result=$(echo -e "$command\nexit" | ./minishell 2>&1 | grep -v "minishell>" | grep -v "^$")
    
    if [[ "$result" == *"$expected"* ]] || [[ -z "$expected" ]]; then
        echo -e "${GREEN}✓ PASS${RESET}"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗ FAIL${RESET}"
        echo -e "${YELLOW}Expected: $expected${RESET}"
        echo -e "${YELLOW}Got: $result${RESET}"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

test_file_output() {
    local name="$1"
    local command="$2"
    local file="$3"
    local expected="$4"
    
    TOTAL=$((TOTAL + 1))
    
    echo -e "${BLUE}Testing:${RESET} $name"
    
    rm -f "$file"
    echo -e "$command\nexit" | ./minishell > /dev/null 2>&1
    
    if [ -f "$file" ] && grep -q "$expected" "$file"; then
        echo -e "${GREEN}✓ PASS${RESET}"
        PASSED=$((PASSED + 1))
        rm -f "$file"
        return 0
    else
        echo -e "${RED}✗ FAIL${RESET}"
        if [ -f "$file" ]; then
            echo -e "${YELLOW}File content: $(cat $file)${RESET}"
        else
            echo -e "${YELLOW}File not created${RESET}"
        fi
        rm -f "$file"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo -e "${BOLD}${BLUE}================================================${RESET}"
echo -e "${BOLD}    MINISHELL QUICK TEST SUITE${RESET}"
echo -e "${BOLD}${BLUE}================================================${RESET}\n"

# Check if minishell exists
if [ ! -f ./minishell ]; then
    echo -e "${RED}Error: ./minishell not found${RESET}"
    echo "Please compile minishell first: make"
    exit 1
fi

echo -e "\n${BOLD}=== SIMPLE COMMANDS ===${RESET}"
test_command "echo test" "echo test" "test"
test_command "pwd" "pwd" "/"
test_command "Empty command" "" ""

echo -e "\n${BOLD}=== ECHO BUILTIN ===${RESET}"
test_command "echo hello world" "echo hello world" "hello world"
test_command "echo -n (no newline)" "echo -n test" "test"
test_command "echo with quotes" "echo \"hello world\"" "hello world"

echo -e "\n${BOLD}=== ENVIRONMENT VARIABLES ===${RESET}"
test_command "Export variable" "export TEST=value\nenv" "TEST=value"
test_command "Variable expansion" "export VAR=hello\necho \$VAR" "hello"
test_command "Unset variable" "export TEST=value\nunset TEST\nenv" ""

echo -e "\n${BOLD}=== RETURN VALUE ===${RESET}"
test_command "\$? after success" "echo test\necho \$?" "0"
test_command "\$? after failure" "/bin/ls nofile_xyz\necho \$?" "1"

echo -e "\n${BOLD}=== QUOTES ===${RESET}"
test_command "Double quotes" "echo \"hello world\"" "hello world"
test_command "Single quotes" "echo 'hello world'" "hello world"
test_command "Single quotes block expansion" "echo '\$USER'" "\$USER"

echo -e "\n${BOLD}=== REDIRECTIONS ===${RESET}"
test_file_output "Output redirect >" "echo test > /tmp/minishell_test.txt" "/tmp/minishell_test.txt" "test"
test_file_output "Append redirect >>" "echo first > /tmp/minishell_test.txt\necho second >> /tmp/minishell_test.txt" "/tmp/minishell_test.txt" "second"
echo "input test" > /tmp/minishell_input.txt
test_command "Input redirect <" "cat < /tmp/minishell_input.txt" "input test"
rm -f /tmp/minishell_input.txt

echo -e "\n${BOLD}=== PIPES ===${RESET}"
test_command "Basic pipe" "echo hello | cat" "hello"
test_command "ls | grep" "ls | grep Makefile" "Makefile"
test_command "Multi-pipe" "echo test | cat | cat" "test"

echo -e "\n${BOLD}=== PIPES + REDIRECTIONS ===${RESET}"
test_file_output "Pipe with redirect" "echo test | cat > /tmp/minishell_pipe.txt" "/tmp/minishell_pipe.txt" "test"
test_file_output "ls | grep > file" "ls | grep Makefile > /tmp/minishell_ls.txt" "/tmp/minishell_ls.txt" "Makefile"

echo -e "\n${BOLD}=== CD/PWD ===${RESET}"
test_command "cd and pwd" "cd /tmp\npwd" "/tmp"
test_command "cd .." "cd ..\npwd" "/"
test_command "cd ." "cd .\npwd" "/"

# Summary
echo -e "\n${BOLD}${BLUE}================================================${RESET}"
echo -e "${BOLD}SUMMARY${RESET}"
echo -e "${BOLD}${BLUE}================================================${RESET}"
echo -e "${GREEN}Passed:  $PASSED/$TOTAL${RESET}"
echo -e "${RED}Failed:  $FAILED/$TOTAL${RESET}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}${BOLD}ALL TESTS PASSED! ✓${RESET}"
    exit 0
else
    echo -e "\n${RED}${BOLD}SOME TESTS FAILED ✗${RESET}"
    exit 1
fi
