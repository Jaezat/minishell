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
        echo -e "      ${BOLD}CMD:${RESET}      $input"
        if [[ -n "$VERBOSE" ]]; then
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
        echo -e "      ${BOLD}CMD:${RESET}      $input"
        if [[ -n "$VERBOSE" ]]; then
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
        echo -e "      ${BOLD}CMD:${RESET}      $input"
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
#  QUOTES
# ============================================================
section "QUOTES"

compare_with_bash 'echo with mixed quoting styles' \
    '/bin/echo "hello world " he"ll"o '"'"''"'"'world'"'"''"'"' '"'"'hello "world"'"'"''

compare_with_bash 'echo empty string then hello' \
    '/bin/echo "" hello'

compare_with_bash 'echo hello then quoted &&' \
    '/bin/echo hello "&&" /bin/echo bye'

compare_with_bash 'echo hello with single-quoted &&' \
    "/bin/echo hello '&& /bin/echo bye'"

compare_with_bash 'single-quoted command prefix' \
    "'/bin/echo hello &&' /bin/echo bye"

syntax_error_test 'unclosed double quote' \
    '/bin/echo "hello'

syntax_error_test 'mixed unclosed quote' \
    "/bin/echo hel\"lo'"

compare_with_bash 'echo with empty quotes between words' \
    'echo hi"" "" "" ""there'

compare_with_bash 'empty double quotes alone' \
    '""'

compare_with_bash "echo \"a' \"" \
    "echo \"a' \""

compare_with_bash 'echo "(a'"'"' )"' \
    'echo "(a'"'"' )"'

compare_with_bash 'ls"-l" quoted flag' \
    'ls"-l"'

compare_with_bash 'echo hello'"'"'world'"'"' single-quote concat' \
    "echo hello'world'"

compare_with_bash 'echo hello""world empty-quote concat' \
    'echo hello""world'

compare_with_bash 'echo special chars in double quotes' \
    'echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $ \ <<"'

# ============================================================
#  SIMPLE COMMANDS
# ============================================================
section "SIMPLE COMMANDS"

compare_with_bash 'ls -l' \
    'ls -l'

compare_with_bash 'cat Makefile' \
    'cat Makefile'

compare_with_bash 'unknown command foo' \
    'foo'

# ============================================================
#  REDIRECTIONS
# ============================================================
section "REDIRECTIONS"

compare_with_bash 'echo hi to file, ls append, cat file' \
    '/bin/echo "hi" > test/file1
ls >> test/file1
cat test/file1'

compare_with_bash 'grep append, cat chain' \
    'rm -f test/file1 test/file2
grep main src/main.c >> test/file1
cat test/file1 >> test/file2
cat test/file1
cat test/file2'

compare_with_bash 'echo empty str redirect with extra arg' \
    '/bin/echo "" > test/file1 hello
cat test/file1'

compare_with_bash 'redirect before command' \
    '>test/file1 /bin/echo hello
cat test/file1'

compare_with_bash 'bare redirect creates empty file' \
    '>test/file1
cat test/file1'

compare_with_bash 'grep with input redirect' \
    'grep main < src/main.c'

compare_with_bash 'wc -l with input redirect' \
    'wc -l < Makefile'

compare_with_bash 'wc -l redirect from missing file' \
    'wc -l < foo'

compare_with_bash 'sort with input redirect' \
    'rm -f test/file1
/bin/echo -e "4\n2\n1\n3" > test/file1
sort < test/file1'

compare_with_bash 'cat Makefile to file1 and append to file2' \
    'rm -f test/file1
echo hello > test/file2
cat < Makefile > test/file1 >> test/file2'

compare_with_bash 'failed cmd with two output redirects' \
    'foo > test/file1 2> test/file2
cat test/file1
cat test/file2'

syntax_error_test 'ls with bare >' \
    'ls >'

syntax_error_test 'cat with bare <' \
    'cat <'

compare_with_bash 'redirect to literal > filename' \
    'echo hi >">" test/file1
ls
rm '"'"'>'"'"''

compare_with_bash 'bare < Makefile' \
    '< Makefile'

syntax_error_test 'cmd > |' \
    'cmd > |'

syntax_error_test 'hello ||||' \
    'hello ||||'

syntax_error_test 'echo HI > > out' \
    'echo HI >    > out'

compare_with_bash '< file cat' \
    '< file cat'

compare_with_bash 'ls > file1 file2 (extra arg)' \
    'ls > file1 file2
rm file1'

compare_with_bash 'ls > file | ls then cat file' \
    'ls > file | ls
cat file
rm file'

compare_with_bash 'export A, echo to out via pipe chain' \
    'export A="hi there"
echo $A > out | unset A | echo $A >> out | export A=FUCK | echo $A >> out
cat out
rm out'

compare_with_bash 'ls >txt -l then cat' \
    'ls >txt -l
cat txt
rm txt'

compare_with_bash 'echo hi >txt banana then cat' \
    'echo hi >txt banana
cat txt
rm txt'

compare_with_bash 'echo hi to two files' \
    'echo hi >"file1" > file2
cat file1
cat file2
rm file1 file2'

compare_with_bash 'grep hi < Makefile' \
    'grep hi < Makefile'

compare_with_bash 'grep with quoted infile then real redirect' \
    'grep hi "<infile" < Makefile'

compare_with_bash 'echo with < redirect and extra args' \
    'echo hi < Makefile bye bye'

compare_with_bash 'grep with two input redirects' \
    'grep h < Makefile < src/main'

compare_with_bash 'echo with < redirect string' \
    'echo < Makefile "bonjour       42"'

compare_with_bash 'cat from file with spaces' \
    'touch "file with spaces"
cat <"file with spaces"
rm "file with spaces"'

compare_with_bash 'cat from concatenated quoted filename' \
    'cat <"1""2""3""4""5"'

compare_with_bash 'echo with multiple missing/present redirects' \
    'echo <Makefile <missing <src/main.c'

compare_with_bash 'echo missing redirect order' \
    'echo <missing <src/main.c <missing'

compare_with_bash 'echo <Makefile piped to cat < src/main.c' \
    'echo <Makefile | cat < src/main.c'

compare_with_bash 'echo <Makefile piped to cat src/main.c' \
    'echo <Makefile | cat src/main.c'

compare_with_bash 'echo <Makefile piped to echo <src/main.c' \
    'echo <Makefile | echo <src/main.c'

compare_with_bash 'echo hi piped to cat <src/main.c' \
    'echo hi | cat <src/main.c'

compare_with_bash 'cat < src/main piped to echo hi' \
    'cat < src/main | echo hi'

compare_with_bash 'cat from missing file' \
    'cat <missing'

compare_with_bash 'cat missing piped to cat' \
    'cat <missing | cat'

compare_with_bash 'cat missing piped to echo oi' \
    'cat <missing | echo oi'

compare_with_bash 'cat missing piped to cat <Makefile' \
    'cat <missing | cat <Makefile'

compare_with_bash 'echo with two missing redirects piped to echo 42' \
    'echo <123 <456 hi | echo 42'

compare_with_bash 'ls to file1 then no_permission, cat both' \
    'ls >test/file1 >test/no_permission
cat test/file1
cat test/no_permission
rm -f test/file1'

compare_with_bash 'ls to no_permission then file1, cat both' \
    'ls >test/no_permission >test/file1
cat test/no_permission
cat test/file1
rm -f test/file1'

compare_with_bash 'echo hi > file | echo bye, cat file' \
    'echo hi >file | echo bye
cat file
rm file'

compare_with_bash 'echo hi | echo > file, cat file' \
    'echo hi | echo >file
cat file
rm file'

compare_with_bash 'echo hi >file1 | echo bye >file2, cat both' \
    'echo hi >file1 | echo bye >file2
cat file1
cat file2
rm file1 file2'

compare_with_bash 'cat <Makefile >file, cat file' \
    'cat <Makefile >file
cat file
rm file'

compare_with_bash 'cat >file <missing, cat file' \
    'cat >file <missing
cat file
rm file'

compare_with_bash 'cat <missing >file, cat file' \
    'cat <missing >file
cat file
rm file'

compare_with_bash 'echo hi > file, ls >>file, cat file' \
    'echo hi > file
ls >>     file
cat file
rm file'

compare_with_bash 'ls >file >>file, cat file' \
    'ls >file >>file
cat file
rm file'

compare_with_bash 'ls >file >>file >file, cat file' \
    'ls >file >>file >file
cat file
rm file'

compare_with_bash 'ls >>file1 >>file2, cat both' \
    'ls >>file1 >>file2
cat file1
cat file2
rm file1 file2'

compare_with_bash 'ls missing redirect to no_permission and file1' \
    'ls <missing >>test/no_permission >>test/file1
cat test/file1
rm -f test/file1
ls >>test/no_permission >>test/file1 <missing
cat test/file1
rm -f test/file1'

compare_with_bash 'cat < Makefile|ls' \
    'cat < Makefile|ls'

compare_with_bash 'echo $>? then rm ?' \
    'echo $>?
rm ?'

compare_with_bash 'cd >> test/file1, cd -, cat' \
    'cd >> test/file1
cd -
cat test/file1
rm -f test/file1'

syntax_error_test 'bare >' \
    '>'

syntax_error_test 'bare >>' \
    '>>'

syntax_error_test 'bare >>>' \
    '>>>'

syntax_error_test 'bare <' \
    '<'

syntax_error_test 'bare <<' \
    '<<'

syntax_error_test 'bare <<<' \
    '<<<'

syntax_error_test 'echo hi | | < >>' \
    'echo hi |    |  < >>'

syntax_error_test 'echo hi | < >> | |' \
    'echo hi |  < >>   |  |'

syntax_error_test 'bare &' \
    '&'

# ============================================================
#  HEREDOCS
# ============================================================
section "HEREDOCS"

compare_with_bash 'cat heredoc1.txt to file1' \
    'cat test/heredoc1.txt > test/file1
cat test/file1'

compare_with_bash 'cat heredoc2.txt' \
    'cat test/heredoc2.txt'

compare_with_bash 'cat heredoc3.txt' \
    'cat test/heredoc3.txt'

compare_with_bash 'cat heredoc9.txt' \
    'cat test/heredoc9.txt'

compare_with_bash 'cat heredoc10.txt' \
    'cat test/heredoc10.txt'

compare_with_bash 'cat heredoc11.txt' \
    'cat test/heredoc11.txt'

compare_with_bash 'cat heredoc12.txt' \
    'cat test/heredoc12.txt'

compare_with_bash 'cat heredoc13.txt' \
    'cat test/heredoc13.txt'

compare_with_bash 'cat heredoc4.txt' \
    'cat test/heredoc4.txt'

compare_with_bash 'cat heredoc5.txt' \
    'cat test/heredoc5.txt'

compare_with_bash 'cat heredoc6.txt' \
    'cat test/heredoc6.txt'

compare_with_bash 'cat heredoc7.txt' \
    'cat test/heredoc7.txt'

compare_with_bash 'cat heredoc8.txt' \
    'cat test/heredoc8.txt'

# ============================================================
#  VARIABLE EXPANSION
# ============================================================
section "VARIABLE EXPANSION"

compare_with_bash 'echo $USER with various quoting' \
    '/bin/echo $USER "$USER" '"'"'$USER'"'"' "hello $USER" "$"USER "$US"ER "$""USER"'

compare_with_bash 'echo bare $ in various contexts' \
    '/bin/echo $ "$" '"'"'$'"'"' $"hello"'

compare_with_bash 'echo $? variations' \
    '/bin/echo $? $?? $       ? $?HELLO'

compare_with_bash 'echo nonexistent var' \
    '/bin/echo "hello $NONE" hello $NONE'

compare_with_bash '$NONE as command prefix' \
    '$NONE /bin/echo hello'

compare_with_bash '$(echo echo hello) command substitution' \
    '$(echo echo hello)'

compare_with_bash 'echo $((1+2)) arithmetic expansion' \
    'echo $((1+2))'

compare_with_bash 'echo tilde variations' \
    '/bin/echo -e "~\n~+\n~-\n~/hi\n~bhb\n"~"\n'"'"'~'"'"'\na~"'

compare_with_bash 'export VAR=ls src then run $VAR' \
    'export VAR="ls src"
$VAR
"$VAR"'

compare_with_bash 'export chain VAR2=$VAR' \
    'export VAR="hi"
export VAR2=$VAR
/bin/echo $VAR2'

compare_with_bash 'export empty VAR echo' \
    'export VAR=""
/bin/echo $VAR'

compare_with_bash 'export VAR with quoted echo command' \
    'export VAR='"'"'"echo hello"'"'"'
$VAR'

compare_with_bash 'export VAR=~ echo $VAR' \
    'export VAR=~
echo $VAR'

compare_with_bash 'export VAR="ho bye" ec$VAR' \
    'export VAR="ho bye"
ec$VAR
ec"$VAR"'

compare_with_bash 'export VAR long string echo' \
    'export VAR="this is a beautiful shell"
/bin/echo $VAR'

compare_with_bash 'echo hello redirect to $USER and $NONE' \
    '/bin/echo hello > $USER
/bin/echo hello > $NONE
rm $USER'

compare_with_bash '"$NONE" as command' \
    '"$NONE" /bin/echo hello'

compare_with_bash 'echo "$NONE""Makefile"' \
    '/bin/echo "$NONE""Makefile"'

compare_with_bash 'echo $1HOME $42HOME' \
    '/bin/echo $1HOME
/bin/echo $42HOME'

compare_with_bash 'echo $SHLVL with quoting' \
    'echo $SHLVL "$SHLVL" '"'"'$SHLVL'"'"' "$SH'"'"''"'"'LVL"'

compare_with_bash 'a=1 b=2 echo $a $b (no export)' \
    'a=1
b=2
echo $a $b'

compare_with_bash 'echo mixed quote dollar strings' \
    'echo "echo $'"'"'NONE'"'"'" '"'"'echo $"NONE"'"'"''

compare_with_bash 'cd obj echo $PWD $OLDPWD' \
    'cd obj
echo $PWD $OLDPWD
cd ..'

compare_with_bash "echo with nested quoting around \$USER" \
    "echo \"'\$USER'\"'\"'\$USER\"'\"'"

# ============================================================
#  WILDCARDS
# ============================================================
section "WILDCARDS"

compare_with_bash 'ls *f' \
    'ls *f'

compare_with_bash 'ls *f*' \
    'ls *f*'

compare_with_bash 'ls **f**' \
    'ls **f**'

compare_with_bash 'ls *' \
    'ls *'

compare_with_bash 'ls .*' \
    'ls .*'

compare_with_bash 'ls "*" (quoted glob)' \
    'ls "*"'

compare_with_bash '/bin/echo *ile *in* and cat < *ile' \
    '/bin/echo *ile *in*
cat < *ile'

compare_with_bash '/bin/echo Make* vs Make"*"' \
    '/bin/echo Make*
/bin/echo Make"*"'

compare_with_bash '/bin/echo *on*' \
    '/bin/echo *on*'

compare_with_bash 'export VAR=*ile echo $VAR' \
    'export VAR=*ile
/bin/echo $VAR'

compare_with_bash 'export VAR=*in* echo $VAR' \
    'export VAR=*in*
/bin/echo $VAR'

compare_with_bash '/bin/echo hello > *in*' \
    '/bin/echo hello > *in*'

compare_with_bash 'export VAR="hi *file" echo both forms' \
    'export VAR="hi *file"
/bin/echo $VAR
/bin/echo "$VAR"'

compare_with_bash 'export VAR=""*ile"" echo $VAR' \
    'export VAR=""*ile""
/bin/echo $VAR'

compare_with_bash "export VAR='*ile' echo \$VAR" \
    "export VAR='*ile'
/bin/echo \$VAR"

compare_with_bash "echo 'bye *' mini* bye" \
    "echo 'bye *' mini* bye"

compare_with_bash '"*" as command' \
    '"*"'

compare_with_bash 'echo * | awk sorted' \
    "echo * | awk -v RS=\" \" '{print}' | sort"

# ============================================================
#  ECHO
# ============================================================
section "ECHO"

compare_with_bash 'echo -n bye' \
    'echo -n bye'

compare_with_bash 'echo -nnn hello' \
    'echo -nnn hello'

compare_with_bash 'echo -f hello' \
    'echo -f hello'

compare_with_bash 'echo -nn -nt hello' \
    'echo -nn -nt hello'

compare_with_bash 'echo --n World' \
    'echo --n World'

compare_with_bash 'echo - "" "  " hello' \
    'echo - "" "  " hello'

# ============================================================
#  CD & PWD
# ============================================================
section "CD & PWD"

compare_with_bash 'cd with no args, pwd, cd -, pwd' \
    'cd
pwd
cd -
pwd'

compare_with_bash 'cd ~/Documents pwd cd -' \
    'cd ~/Documents
pwd
cd -'

compare_with_bash 'cd test pwd cd .. pwd' \
    'cd test
pwd
cd ..
pwd'

compare_with_bash 'pwd -f (invalid flag)' \
    'pwd -f'

compare_with_bash 'cd -f (invalid flag)' \
    'cd -f'

compare_with_bash 'cd with two args' \
    'cd src include'

compare_with_bash 'cd to nonexistent dir' \
    'cd none'

compare_with_bash 'pwd oi (extra arg)' \
    'pwd oi'

compare_with_bash 'cd "some folder"' \
    'cd "some folder"'

compare_with_bash 'cd $PWD' \
    'cd $PWD'

compare_with_bash 'cd $PWD hi (extra arg)' \
    'cd $PWD hi'

compare_with_bash 'cd to numeric dir name' \
    'cd 123123'

# ============================================================
#  EXPORT, UNSET & ENV
# ============================================================
section "EXPORT, UNSET & ENV"

compare_with_bash 'export VAR=42, grep, unset' \
    'export VAR=42
export | grep VAR
unset VAR'

compare_with_bash 'env with extra arg' \
    'env hello'

compare_with_bash 'env -w (invalid flag)' \
    'env -w'

compare_with_bash 'unset -w (invalid flag)' \
    'unset -w'

compare_with_bash 'export -l (invalid flag)' \
    'export -l'

compare_with_bash 'export a = b (spaces around =)' \
    'export a = b'

compare_with_bash 'export VAR with spaces, echo, unset' \
    'export VAR="h   e    l l      o"
echo $VAR
unset VAR'

compare_with_bash 'export AAA= env grep unset' \
    'export AAA=
env | grep AAA
unset AAA'

compare_with_bash 'export AAA=BBB=CCC echo unset' \
    'export AAA=BBB=CCC
echo $AAA
unset AAA'

compare_with_bash 'export VAR@@@ (invalid name)' \
    'export VAR@@@'

compare_with_bash 'export VAR=var@@@ echo unset' \
    'export VAR=var@@@
echo $VAR
unset VAR'

compare_with_bash 'unset HOME then cd' \
    'unset HOME
cd'

compare_with_bash 'touch ls, ls, rm ls' \
    'touch ls
ls
rm ls'

compare_with_bash 'unset USER PATH PWD then ls and /bin/ls' \
    'unset USER
unset PATH
unset PWD
ls
/bin/ls'

compare_with_bash 'export var (no value) echo $var unset' \
    'export var
echo $var
unset var'

compare_with_bash 'export A- (invalid)' \
    'export A-'

compare_with_bash 'export HELLO=123 A, echo, unset' \
    'export HELLO=123 A
echo $HELLO
unset HELLO'

compare_with_bash 'export HELLO="123 A-" echo unset' \
    'export HELLO="123 A-"
echo $HELLO
unset HELLO'

compare_with_bash 'export hello world, echo, env grep' \
    'export hello world
echo $hello $world
env | grep hello
env | grep world'

compare_with_bash 'export HELLO-=123 (invalid)' \
    'export HELLO-=123'

compare_with_bash 'export 123 (invalid)' \
    'export 123'

compare_with_bash 'unset with no args' \
    'unset'

compare_with_bash 'unset NONE (nonexistent)' \
    'unset NONE'

compare_with_bash 'unset SHELL, env grep SHELL' \
    'unset SHELL
env | grep SHELL'

compare_with_bash 'a==1 echo $a' \
    'a==1
echo $a'

compare_with_bash 'export a, export, env' \
    'export a
export
env'

# ============================================================
#  EXIT
# ============================================================
section "EXIT"

run_test "exit (no arg)"   "exit"    "" "0"
run_test "exit 2"          "exit 2"  "" "2"
run_test "exit -2"         "exit -2" "" "254"
run_test "exit p (non-numeric)" "exit p" "" "2"
run_test "exit a 1 (too many args)" "exit a 1" "" "1"
run_test "exit 298 (overflow mod 256)" "exit 298" "" "42"
run_test "exit +100"       "exit +100"   "" "100"
run_test 'exit "+100"'     'exit "+100"' "" "100"
run_test 'exit +"100"'     'exit +"100"' "" "2"
run_test "exit 9223372036854775808 (overflow)" "exit 9223372036854775808" "" "2"
run_test "exit a a (too many args)"             "exit a a" "" "1"

# ============================================================
#  PIPES
# ============================================================
section "PIPES"

compare_with_bash 'ls | grep minishell' \
    'ls | grep minishell'

compare_with_bash 'cat include/minishell.h | grep ");"' \
    'cat include/minishell.h | grep ");"'

compare_with_bash 'ls | wc -l' \
    'ls | wc -l'

compare_with_bash 'cat Makefile | head -n 5' \
    'cat Makefile | head -n 5'

compare_with_bash 'grep include | sort | uniq' \
    'grep -i include src/main.c | sort | uniq'

compare_with_bash 'cat Makefile pipeline' \
    "cat Makefile | grep SRC | cut -d'=' -f2 | tr ' ' '\n' | wc -l"

compare_with_bash 'find ~ -type f | wc -l' \
    'find ~ -type f | wc -l'

compare_with_bash 'ls | sort | uniq | rev | rev' \
    'ls | sort | uniq | rev | rev'

compare_with_bash 'cat Makefile tr uppercase grep wc' \
    'cat Makefile | tr a-z A-Z | grep SRC | wc -c'

compare_with_bash 'ls /bin | grep sh | head | tail' \
    'ls /bin | grep sh | head -n 3 | tail -n 1'

compare_with_bash 'seq 1 100 | grep 42 | wc -l' \
    'seq 1 100 | grep 42 | wc -l'

syntax_error_test 'pipe at start | ls' \
    '| ls'

syntax_error_test 'ls | | wc' \
    'ls | | wc'

syntax_error_test 'ls | (trailing)' \
    'ls |'

syntax_error_test '| | |' \
    '| | |'

compare_with_bash 'foo | wc -l' \
    'foo | wc -l'

compare_with_bash 'ls | foo' \
    'ls | foo'

compare_with_bash 'a | b | c | d | e | f | g' \
    'a | b | c | d | e | f | g'

compare_with_bash 'hello | ls' \
    'hello | ls'

compare_with_bash 'ls | hello' \
    'ls | hello'

compare_with_bash 'echo to lal | ls | cat lal' \
    'echo hiiiiiii > lal | ls | cat lal
cat lal
rm lal'

compare_with_bash 'echo hi | ls | grep Makefile | wc > out2 | cat out2' \
    'echo hi | ls | grep Makefile | wc -l > out2 | cat out2
rm out2'

compare_with_bash 'chmod 000 file, < aaa wc | echo Hi' \
    'touch aaa
chmod 000 aaa
< aaa  wc | echo Hi
rm -f aaa'

compare_with_bash 'ls | wc | echo > out.txt | wc -l' \
    'ls | wc | echo > out.txt | wc -l
cat out.txt
rm out.txt'

syntax_error_test 'cat <| ls' \
    'cat    <| ls'

syntax_error_test 'echo hi | >' \
    'echo hi | >'

syntax_error_test 'echo hi | > >>' \
    'echo hi | > >>'

syntax_error_test 'echo hi | < |' \
    'echo hi | < |'

syntax_error_test 'echo hi | |' \
    'echo hi |   |'

compare_with_bash 'echo hi | "|"' \
    'echo hi |  "|"'

compare_with_bash 'ls -l "" | >> hi echo cat $HOME' \
    'ls -l "" "HI'"'"'LOL'"'"'|" | >> hi echo cat $HOME
cat hi
rm hi'

compare_with_bash 'cat file.txt | grep error | wc -l' \
    "cat file.txt | grep 'error' | wc -l"

compare_with_bash '(echo ola) | cat | cat | cat' \
    '(echo ola) | cat | cat | cat'

compare_with_bash '(echo ola) | cat | cat | ls' \
    '(echo ola) | cat | cat | ls'

compare_with_bash '(echo ola) | cat | cat | ls | wc -l' \
    '(echo ola) | cat | cat | ls | wc -l'

# ============================================================
#  LOGICAL OPERATORS & PARENTHESES
# ============================================================
section "LOGICAL OPERATORS & PARENTHESES"

compare_with_bash 'ls && cat Makefile' \
    'ls && cat Makefile'

compare_with_bash 'cat foo || echo not found' \
    'cat foo || echo "not found"'

compare_with_bash '(/bin/echo hello && /bin/echo world) | wc -l' \
    '(/bin/echo hello && /bin/echo world) | wc -l'

compare_with_bash '/bin/echo hello && /bin/echo world | wc -l' \
    '/bin/echo hello && /bin/echo world | wc -l'

compare_with_bash '(ls)' \
    '(ls)'

compare_with_bash '((ls))' \
    '((ls))'

compare_with_bash '( (ls) )' \
    '( (ls) )'

compare_with_bash '((ls) )' \
    '((ls) )'

compare_with_bash '( (ls))' \
    '( (ls))'

compare_with_bash '/bin/echo a && /bin/echo b || /bin/echo c' \
    '/bin/echo a && /bin/echo b || /bin/echo c'

compare_with_bash '/bin/echo a || /bin/echo b && /bin/echo c' \
    '/bin/echo a || /bin/echo b && /bin/echo c'

syntax_error_test '() empty parens' \
    '()'

syntax_error_test 'ls) unmatched paren' \
    'ls)'

compare_with_bash '(ls) /bin/echo hello' \
    '(ls) /bin/echo hello'

compare_with_bash '(ls) (ls)' \
    '(ls) (ls)'

syntax_error_test '|| ls (leading ||)' \
    '|| ls'

syntax_error_test '&& ls (leading &&)' \
    '&& ls'

syntax_error_test '|| && |' \
    '|| && |'

syntax_error_test 'ls ||| wc' \
    'ls ||| wc'

syntax_error_test '/bin/echo "start" && (ls -l | grep minishell) >' \
    '/bin/echo "start" && (ls -l | grep minishell) >'

syntax_error_test '(ls (unclosed)' \
    '(ls'

syntax_error_test '/bin/echo ( (invalid)' \
    '/bin/echo ('

syntax_error_test 'ls && (trailing)' \
    'ls &&'

syntax_error_test 'ls || (trailing)' \
    'ls ||'

compare_with_bash '/bin/echo start && (ls | grep minishell) && /bin/echo end' \
    '/bin/echo "start" && (ls -l | grep minishell) && /bin/echo end'

compare_with_bash '/bin/echo start && (ls | grep) > file && cat file' \
    '/bin/echo "start" && (ls -l | grep minishell) > test/file1 && cat test/file1'

compare_with_bash '/bin/echo start && (ls | grep) | /bin/echo end' \
    '/bin/echo "start" && (ls -l | grep minishell) | /bin/echo end'

compare_with_bash 'ls -l > file && (cat < file || echo fail) | wc -l' \
    'ls -l > test/file1 && (cat < test/file1 || echo fail) | wc -l'

compare_with_bash '(exit 99) || echo $?' \
    '(exit 99) || echo $?'

compare_with_bash 'nested parens with && and ||' \
    '((echo ola && adsdsa) || ((echo ola && aedsffds) || echo done))'

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
