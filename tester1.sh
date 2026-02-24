#!/bin/bash

# ============================================================
#  usage:
#    ./tester.sh ./minishell
#    ./tester.sh ./minishell --verbose
# ============================================================

MINISHELL=${1:-"./minishell"}
VERBOSE=${2:-""}
PASS=0
FAIL=0
TMPDIR_TEST=$(mktemp -d)
PROMPT="minishell\$>"

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

# ============================================================
#  Helpers
# ============================================================

cleanup() {
    rm -rf "$TMPDIR_TEST"
}
trap cleanup EXIT

section() {
    echo ""
    echo -e "${CYAN}${BOLD}══════════════════════════════════════${RESET}"
    echo -e "${CYAN}${BOLD}  $1${RESET}"
    echo -e "${CYAN}${BOLD}══════════════════════════════════════${RESET}"
}

# compare_with_bash: runs in both minishell and bash, compares output + exit code
compare_with_bash() {
    local desc="$1"
    local input="$2"

    local actual_out
    actual_out=$(echo "$input" | "$MINISHELL" 2>/dev/null | grep -v "^$PROMPT")
    local actual_exit=$?

    local bash_out
    bash_out=$(echo "$input" | bash 2>/dev/null)
    local bash_exit=$?

    if [[ "$actual_out" == "$bash_out" && "$actual_exit" == "$bash_exit" ]]; then
        echo -e "  ${GREEN}✓${RESET} $desc"
        ((PASS++))
    else
        echo -e "  ${RED}✗${RESET} $desc"
        if [[ -n "$VERBOSE" ]]; then
            echo -e "      ${BOLD}Input:${RESET}    $input"
            echo -e "      ${BOLD}Bash:${RESET}     $(echo "$bash_out" | head -3) [exit $bash_exit]"
            echo -e "      ${BOLD}Got:${RESET}      $(echo "$actual_out" | head -3) [exit $actual_exit]"
        fi
        ((FAIL++))
    fi
}

# run_test: specify expected output and/or exit code explicitly
run_test() {
    local desc="$1"
    local input="$2"
    local expected_out="${3:-SKIP_OUTPUT}"
    local expected_exit="${4:-}"

    local actual_out
    actual_out=$(echo "$input" | "$MINISHELL" 2>/dev/null | grep -v "^$PROMPT")
    local actual_exit=$?

    local ok=1

    if [[ "$expected_out" != "SKIP_OUTPUT" && "$actual_out" != "$expected_out" ]]; then
        ok=0
    fi

    if [[ -n "$expected_exit" && "$actual_exit" != "$expected_exit" ]]; then
        ok=0
    fi

    if [[ $ok -eq 1 ]]; then
        echo -e "  ${GREEN}✓${RESET} $desc"
        ((PASS++))
    else
        echo -e "  ${RED}✗${RESET} $desc"
        if [[ -n "$VERBOSE" ]]; then
            echo -e "      ${BOLD}Input:${RESET}    $input"
            if [[ "$expected_out" != "SKIP_OUTPUT" ]]; then
                echo -e "      ${BOLD}Expected:${RESET} $(echo "$expected_out" | head -3)"
                echo -e "      ${BOLD}Got:${RESET}      $(echo "$actual_out" | head -3)"
            fi
            if [[ -n "$expected_exit" ]]; then
                echo -e "      ${BOLD}Exit exp:${RESET} $expected_exit  ${BOLD}Got:${RESET} $actual_exit"
            fi
        fi
        ((FAIL++))
    fi
}

# syntax_error_test: expects non-zero exit (syntax error)
syntax_error_test() {
    local desc="$1"
    local input="$2"

    echo "$input" | "$MINISHELL" 2>/dev/null | grep -v "^$PROMPT" > /dev/null
    local actual_exit=$?

    if [[ $actual_exit -ne 0 ]]; then
        echo -e "  ${GREEN}✓${RESET} $desc (error caught)"
        ((PASS++))
    else
        echo -e "  ${RED}✗${RESET} $desc (expected error, got exit 0)"
        ((FAIL++))
    fi
}

# ============================================================
#  Check minishell exists
# ============================================================

if [[ ! -f "$MINISHELL" ]]; then
    echo -e "${RED}Error: minishell not found at '$MINISHELL'${RESET}"
    echo "Usage: $0 ./minishell [--verbose]"
    exit 1
fi

echo ""
echo -e "${BOLD}Minishell Tester - 42 (mandatory only)${RESET}"
echo -e "Testing: ${CYAN}$MINISHELL${RESET}"
[[ -n "$VERBOSE" ]] && echo -e "Mode: ${YELLOW}verbose${RESET}"

# ============================================================
#  PARSING - Quotes
# ============================================================
section "PARSING - Quotes"

compare_with_bash "simple echo"                      "echo hello world"
compare_with_bash "double quotes"                    'echo "hello world"'
compare_with_bash "single quotes"                    "echo 'hello world'"
compare_with_bash "single inside double"             "echo \"it's a test\""
compare_with_bash "double inside single"             "echo 'say \"hi\"'"
compare_with_bash "empty double quotes"              'echo ""'
compare_with_bash "empty single quotes"              "echo ''"
compare_with_bash "quote+noquote concat"             'echo "hello"world'
compare_with_bash "noquote+quote concat"             'echo hello"world"'
compare_with_bash "single then double concat"        "echo 'hello'\"world\""
compare_with_bash "spaces inside double quotes"      'echo "hi     world"'
compare_with_bash "spaces inside single quotes"      "echo 'hi     world'"
compare_with_bash "only spaces in quotes"            'echo "   "'

# ============================================================
#  PARSING - Variables
# ============================================================
section "PARSING - Variables / \$"

compare_with_bash "echo \$HOME"                      'echo $HOME'
compare_with_bash "echo \$USER"                      'echo $USER'
compare_with_bash "echo \$PATH"                      'echo $PATH'
compare_with_bash "double quote expands var"         'echo "$HOME"'
compare_with_bash "single quote no expand"           "echo '\$HOME'"
compare_with_bash "dollar alone in double quotes"    'echo "$"'
compare_with_bash "nonexistent var is empty"         'echo $NONEXISTENT_VAR_42'
compare_with_bash "var in middle of string"          'echo "hello $USER world"'
compare_with_bash "two vars concatenated"            'echo "$HOME $USER"'
compare_with_bash "var concat no space"              'echo $HOME$USER'
compare_with_bash "export and echo"                  'export _T42=hello
echo $_T42'
compare_with_bash "export empty val"                 'export _T42=
echo $_T42'
compare_with_bash "unset var becomes empty"          'export _T42=hello
unset _T42
echo $_T42'

# ============================================================
#  PARSING - $?
# ============================================================
section "PARSING - \$?"

compare_with_bash "dollar? after success"            'echo hi > /dev/null
echo $?'
compare_with_bash "dollar? after failure"            'ls /nonexistent_42_xyz 2>/dev/null
echo $?'
compare_with_bash "dollar? itself is 0"              'echo $?
echo $?'
compare_with_bash "dollar? in double quotes"         'echo "$?"'

# ============================================================
#  PARSING - Syntax errors
# ============================================================
section "PARSING - Syntax errors"

syntax_error_test "pipe at start"                   "| echo hi"
syntax_error_test "double pipe"                     "echo hi | | echo bye"
syntax_error_test "pipe at end"                     "echo hi |"
syntax_error_test "redirect missing filename"       "echo hi >"

# ============================================================
#  PIPES
# ============================================================
section "PIPES"

compare_with_bash "simple pipe"                     "echo hi | cat"
compare_with_bash "double pipe"                     "echo hi | cat | cat"
compare_with_bash "many pipes"                      "echo hi | cat | cat | cat | cat"
compare_with_bash "pipe to grep found"              "echo hello | grep hello"
compare_with_bash "pipe to grep not found"          "echo hello | grep world"
compare_with_bash "pipe to wc -c"                   "echo hello | wc -c"
compare_with_bash "pipe to sort"                    "printf 'b\na\nc\n' | sort"
compare_with_bash "pipe chain sort uniq"            "printf 'a\na\nb\n' | sort | uniq"
compare_with_bash "pipe to wc -l"                   "printf 'a\nb\nc\n' | wc -l"
compare_with_bash "cat /dev/null pipe wc"           "cat /dev/null | wc -l"
compare_with_bash "pipe exit code grep found"       'echo hi | grep hi > /dev/null
echo $?'
compare_with_bash "pipe exit code grep not found"   'echo hi | grep bye > /dev/null
echo $?'

# ============================================================
#  REDIRECTIONS - Output >
# ============================================================
section "REDIRECTIONS - Output >"

OUT="$TMPDIR_TEST/out"

run_test "> creates file with content" \
    "echo hi > $OUT
cat $OUT" \
    "hi"

run_test "> overwrites existing file" \
    "echo hi > $OUT
echo bye > $OUT
cat $OUT" \
    "bye"

run_test "> creates empty file" \
    "> $OUT
cat $OUT" \
    ""

run_test "redirect to /dev/null gives no output" \
    "echo hi > /dev/null" \
    ""

run_test "ls redirected to file" \
    "ls /tmp > $OUT
test -s $OUT && echo notempty" \
    "notempty"

# ============================================================
#  REDIRECTIONS - Append >>
# ============================================================
section "REDIRECTIONS - Append >>"

run_test ">> creates file" \
    "echo hi >> $OUT
cat $OUT" \
    "hi"

run_test ">> appends to existing" \
    "echo hi > $OUT
echo world >> $OUT
cat $OUT" \
    "$(printf 'hi\nworld')"

run_test "multiple >> appends" \
    "echo a >> $OUT
echo b >> $OUT
echo c >> $OUT
cat $OUT" \
    "$(printf 'a\nb\nc')"

# ============================================================
#  REDIRECTIONS - Input <
# ============================================================
section "REDIRECTIONS - Input <"

echo "hello world" > "$OUT"

run_test "cat with <" \
    "cat < $OUT" \
    "hello world"

run_test "wc -l with <" \
    "printf 'a\nb\nc\n' > $OUT
wc -l < $OUT" \
    "3"

run_test "redirect before command" \
    "echo test > $OUT
< $OUT cat" \
    "test"

run_test "< and > combined" \
    "echo hello > $OUT
cat < $OUT > ${OUT}2
cat ${OUT}2" \
    "hello"

syntax_error_test "< nonexistent file"              "cat < /nonexistent_file_42_xyz"

# ============================================================
#  REDIRECTIONS - Combinations
# ============================================================
section "REDIRECTIONS - Combinations"

run_test "pipe then redirect out" \
    "echo hello | cat > $OUT
cat $OUT" \
    "hello"

run_test "pipe with grep and redirect" \
    "echo hello > $OUT
cat < $OUT | grep hello > ${OUT}2
cat ${OUT}2" \
    "hello"

# ============================================================
#  HEREDOCS <<
# ============================================================
section "HEREDOCS - Basic"

compare_with_bash "basic heredoc" \
'cat << EOF
hello
EOF'

compare_with_bash "heredoc multiline" \
'cat << EOF
hello
world
bye
EOF'

compare_with_bash "heredoc empty body" \
'cat << EOF
EOF'

compare_with_bash "heredoc empty line in body" \
'cat << EOF

EOF'

compare_with_bash "heredoc expands variables" \
'cat << EOF
$HOME
EOF'

compare_with_bash "heredoc quoted delimiter no expand" \
"cat << 'EOF'
\$HOME
EOF"

compare_with_bash "heredoc lowercase delimiter" \
'cat << eof
hello
eof'

section "HEREDOCS - With pipes and redirections"

compare_with_bash "heredoc piped to grep" \
'cat << EOF | grep hello
hello
world
EOF'

compare_with_bash "heredoc piped chain" \
'cat << EOF | cat | wc -l
a
b
c
EOF'

compare_with_bash "heredoc redirect to file" \
"cat << EOF > $TMPDIR_TEST/hd_out
hello
EOF
cat $TMPDIR_TEST/hd_out"

# ============================================================
#  BUILTINS - echo
# ============================================================
section "BUILTINS - echo"

compare_with_bash "echo no args"              "echo"
compare_with_bash "echo multiple args"        "echo a b c"
compare_with_bash "echo -n"                   "echo -n hi"
compare_with_bash "echo -n no args"           "echo -n"
compare_with_bash "echo empty string"         'echo ""'
compare_with_bash "echo single quotes"        "echo 'hello world'"

# ============================================================
#  BUILTINS - pwd
# ============================================================
section "BUILTINS - pwd"

compare_with_bash "pwd"                       "pwd"
compare_with_bash "pwd after cd"              "cd /tmp
pwd"

# ============================================================
#  BUILTINS - cd
# ============================================================
section "BUILTINS - cd"

compare_with_bash "cd absolute path"          "cd /tmp
pwd"
compare_with_bash "cd home no args"           "cd
pwd"
compare_with_bash "cd relative path"          "cd /tmp
cd ..
pwd"
compare_with_bash "cd nonexistent error code" "cd /nonexistent_42_xyz 2>/dev/null
echo \$?"

# ============================================================
#  BUILTINS - export / unset / env
# ============================================================
section "BUILTINS - export / unset / env"

compare_with_bash "export and echo"           "export _T42=hello
echo \$_T42"
compare_with_bash "export overwrite"          "export _T42=hello
export _T42=bye
echo \$_T42"
compare_with_bash "export empty value"        "export _T42=
echo \$_T42"
compare_with_bash "unset removes var"         "export _T42=hello
unset _T42
echo \$_T42"
compare_with_bash "unset nonexistent no err"  "unset _NONEXISTENT_42
echo \$?"
compare_with_bash "env contains exported var" "export _T42=hello
env | grep _T42"

# ============================================================
#  BUILTINS - exit
# ============================================================
section "BUILTINS - exit"

run_test "exit 0"    "exit 0"    "" "0"
run_test "exit 1"    "exit 1"    "" "1"
run_test "exit 42"   "exit 42"   "" "42"
run_test "exit 255"  "exit 255"  "" "255"

# ============================================================
#  SUMMARY
# ============================================================
echo ""
echo -e "${CYAN}${BOLD}══════════════════════════════════════${RESET}"
echo -e "${BOLD}  RESULTS${RESET}"
echo -e "${CYAN}${BOLD}══════════════════════════════════════${RESET}"
echo -e "  ${GREEN}✓ Passed: $PASS${RESET}"
echo -e "  ${RED}✗ Failed: $FAIL${RESET}"
TOTAL=$((PASS + FAIL))
echo -e "  Total:   $TOTAL"
echo ""
if [[ $FAIL -eq 0 ]]; then
    echo -e "  ${GREEN}${BOLD}All tests passed! 🎉${RESET}"
else
    echo -e "  ${YELLOW}Tip: run with --verbose for details${RESET}"
    echo -e "  ${YELLOW}  $0 $MINISHELL --verbose${RESET}"
fi
echo ""

exit $FAIL
