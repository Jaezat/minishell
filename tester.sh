#!/bin/bash

export LC_ALL=C

# 1. Colors
GREEN="\033[32m"
RED="\033[31m"
RESET="\033[0m"

run_test() {
    CMD="$1"
    
    echo "$CMD" | bash > bash_out 2>&1
    echo "$CMD" | ./minishell > mini_out 2>&1
    
    # Filter out the test files themselves from the output
    grep -v "bash_out" bash_out | grep -v "mini_out" > bash_final
    grep -v "bash_out" mini_out | grep -v "mini_out" > mini_final

    # Diff the clean versions
    DIFF=$(diff bash_final mini_final)
    
    if [ "$DIFF" == "" ]; then
        echo -e "${GREEN}[OK]${RESET} '$CMD'"
    else
        echo -e "${RED}[KO]${RESET} '$CMD'"
        echo "--- Bash Output ---"
        cat bash_final
        echo "--- Mini Output ---"
        cat mini_final
        echo "-------------------"
    fi
    
    # Cleanup temps
    rm bash_final mini_final
}

# 3. The Test Suite
echo "--- Starting Tests ---"

run_test "ls | sort"
run_test "ls -la | sort"
run_test "echo hello world"
run_test "ls | wc -l"
run_test "cat | cat | ls | sort"
run_test "export VAR=123 | echo \$VAR"
run_test "echo \$USER"
run_test "exit 42"
run_test "pwd | cd .. | pwd | cd - | pwd"
run_test "cd | ls -l | sort"
run_test "echo -nnnnn what's up"

# Cleanup
rm bash_out mini_out

