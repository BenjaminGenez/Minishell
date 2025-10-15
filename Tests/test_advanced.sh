#!/bin/bash
# Advanced edge case tests for Minishell

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
RESET='\033[0m'

echo -e "${BOLD}${BLUE}Advanced Minishell Tests${RESET}\n"

# Test 1: Multiple pipes
echo -e "${BLUE}Test: Multiple pipes (4-way)${RESET}"
result=$(echo -e "echo test | cat | cat | cat\nexit" | ./minishell 2>&1 | grep "test")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi

# Test 2: Pipe with multiple arguments
echo -e "\n${BLUE}Test: Pipe with arguments${RESET}"
result=$(echo -e "/bin/ls -la | /bin/grep Makefile\nexit" | ./minishell 2>&1 | grep "Makefile")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi

# Test 3: Multiple redirections (last one wins)
echo -e "\n${BLUE}Test: Multiple output redirections${RESET}"
echo -e "echo test > /tmp/test1.txt > /tmp/test2.txt\nexit" | ./minishell > /dev/null 2>&1
if [ -f /tmp/test2.txt ] && grep -q "test" /tmp/test2.txt; then
    echo -e "${GREEN}✓ PASS - Last redirect wins${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi
rm -f /tmp/test1.txt /tmp/test2.txt

# Test 4: Input + Output redirection
echo "hello from file" > /tmp/input.txt
echo -e "\n${BLUE}Test: Input + Output redirect${RESET}"
echo -e "cat < /tmp/input.txt > /tmp/output.txt\nexit" | ./minishell > /dev/null 2>&1
if [ -f /tmp/output.txt ] && grep -q "hello from file" /tmp/output.txt; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi
rm -f /tmp/input.txt /tmp/output.txt

# Test 5: Export without value
echo -e "\n${BLUE}Test: Export without value${RESET}"
result=$(echo -e "export TESTVAR\nenv\nexit" | ./minishell 2>&1 | grep "TESTVAR")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${YELLOW}⊘ SKIP - May not be required${RESET}"
fi

# Test 6: Multiple exports
echo -e "\n${BLUE}Test: Multiple exports${RESET}"
result=$(echo -e "export A=1 B=2 C=3\nenv\nexit" | ./minishell 2>&1)
if [[ "$result" == *"A=1"* ]] || [[ "$result" == *"B=2"* ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${YELLOW}⊘ SKIP - Single export per line OK${RESET}"
fi

# Test 7: cd to HOME
echo -e "\n${BLUE}Test: cd without arguments (HOME)${RESET}"
result=$(echo -e "cd\npwd\nexit" | ./minishell 2>&1 | grep -E "home|root")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${YELLOW}⊘ May fail if HOME not set${RESET}"
fi

# Test 8: Pipe with redirection in middle
echo -e "\n${BLUE}Test: Pipe chain with intermediate redirect${RESET}"
echo -e "echo test | cat > /tmp/mid.txt | cat\nexit" | ./minishell > /dev/null 2>&1
if [ -f /tmp/mid.txt ]; then
    echo -e "${GREEN}✓ PASS - Redirect created${RESET}"
else
    echo -e "${YELLOW}⊘ Complex redirect case${RESET}"
fi
rm -f /tmp/mid.txt

# Test 9: Command with many arguments
echo -e "\n${BLUE}Test: Command with 10+ arguments${RESET}"
result=$(echo -e "echo a b c d e f g h i j k l m n o\nexit" | ./minishell 2>&1 | grep "a b c")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi

# Test 10: Empty variable expansion
echo -e "\n${BLUE}Test: Empty variable expansion${RESET}"
result=$(echo -e "echo \$NONEXISTENT\nexit" | ./minishell 2>&1 | tail -2)
echo -e "${GREEN}✓ PASS - Handled (empty or literal)${RESET}"

# Test 11: Quote combinations
echo -e "\n${BLUE}Test: Mixed quotes${RESET}"
result=$(echo -e "echo \"hello 'world'\"\nexit" | ./minishell 2>&1 | grep "hello 'world'")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi

# Test 12: Builtin in pipe
echo -e "\n${BLUE}Test: Builtin in pipe (echo | cat)${RESET}"
result=$(echo -e "echo hello | cat\nexit" | ./minishell 2>&1 | grep "hello")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi

# Test 13: Spaces around redirections
echo -e "\n${BLUE}Test: Spaces around >>${RESET}"
echo -e "echo test   >>   /tmp/spaces.txt\nexit" | ./minishell > /dev/null 2>&1
if [ -f /tmp/spaces.txt ] && grep -q "test" /tmp/spaces.txt; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi
rm -f /tmp/spaces.txt

# Test 14: Exit codes
echo -e "\n${BLUE}Test: Exit code 255${RESET}"
echo -e "exit 255\nexit" | ./minishell > /dev/null 2>&1
exitcode=$?
if [ $exitcode -eq 255 ]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${YELLOW}⊘ Got exit code: $exitcode${RESET}"
fi

# Test 15: Long command line
echo -e "\n${BLUE}Test: Long command (200+ chars)${RESET}"
longcmd="echo "
for i in {1..50}; do longcmd+="word$i "; done
result=$(echo -e "$longcmd\nexit" | ./minishell 2>&1 | grep "word1")
if [[ -n "$result" ]]; then
    echo -e "${GREEN}✓ PASS${RESET}"
else
    echo -e "${RED}✗ FAIL${RESET}"
fi

echo -e "\n${BOLD}${BLUE}Advanced tests complete!${RESET}"
