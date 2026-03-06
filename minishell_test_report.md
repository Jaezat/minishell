# Minishell Test Report

**Date:** 2026-03-03
**Total commands tested:** 350
**Commands matching bash:** 338
**Commands with differences:** 12 (4 real bugs, 8 format/timing differences)

---

## REAL BUGS

---

### Bug 1 — `&&` with unclosed single quote

**Command:**
```
/bin/echo hello &&' /bin/echo bye
```

| Shell | Output |
|---|---|
| bash | `hello` then `bash: unexpected EOF while looking for matching '''` |
| minishell | `hello  /bin/echo bye` |

**What's wrong:**
When `&&` is followed by an unclosed single quote, minishell echoes the right-hand side as text instead of producing a syntax/quote error. The token `' /bin/echo bye` has an unclosed `'` — this should be an error, not output.

---

### Bug 2 — Unclosed double quote produces no error

**Command:**
```
/bin/echo "hello
```

| Shell | Output |
|---|---|
| bash | `bash: line 1: unexpected EOF while looking for matching '"'` |
| minishell | `hello` |

**What's wrong:**
Minishell silently ignores the unclosed double quote and runs the command as if the quote were closed. It should produce a syntax error.

---

### Bug 3 — Unclosed single quote produces no error

**Command:**
```
/bin/echo hel"lo"'
```

| Shell | Output |
|---|---|
| bash | `bash: line 1: unexpected EOF while looking for matching '''` |
| minishell | `hello` |

**What's wrong:**
Same as Bug 2 — minishell ignores the trailing unclosed single quote and produces output instead of an error.

---

### Bug 4 — `2>` does not capture command-not-found errors

**Command:**
```
foo > test/file1 2> test/file2
cat test/file2
```

**Bash behavior:**

| Step | Result |
|---|---|
| `foo > test/file1 2> test/file2` | No output (error redirected to test/file2) |
| `cat test/file2` | `bash: line 1: foo: command not found` |

**Minishell behavior:**

| Step | Result |
|---|---|
| `foo > test/file1 2> test/file2` | `minishell: foo: command not found` printed to terminal |
| `cat test/file2` | *(empty)* |

**What's wrong:**
When a command is not found and stderr is redirected with `2>`, bash writes the error from the **child process** (after fork, with stderr already pointing to the file). Minishell writes the error from the **parent shell** before/after forking, so the `2>` redirection has no effect on it. The file is created (truncated) by the redirect, but nothing is written into it.

**Verified with isolated test:**
```sh
# bash
printf 'foo 2>/tmp/t2\n' | bash --norc --noprofile 2>/dev/null
cat /tmp/t2   # → "bash: line 1: foo: command not found"

# minishell
printf 'foo 2>/tmp/t2\n' | ./minishell 2>/dev/null
cat /tmp/t2   # → (empty)
```

---

## NON-BUGS (format differences only)

These differences are **expected and acceptable** — minishell uses its own name and error format, which is standard for a custom shell.

| # | Command | bash output | minishell output | Note |
|---|---|---|---|---|
| 1 | `ls"-l"` | `bash: line N: ls-l: command not found` | `minishell: ls-l: command not found` | prefix only |
| 2 | `EOF'A'"'\|'"` | `bash: line N: EOFA'\|': command not found` | `minishell: EOFA'\|': command not found` | prefix only |
| 3 | `foo` | `bash: line N: foo: command not found` | `minishell: foo: command not found` | prefix only |
| 4 | `wc -l < foo` | `bash: line N: foo: No such file or directory` | `foo: No such file or directory` | prefix + line number |
| 5 | `ls >` | Two-line error with `'ls >'` context shown | One-line syntax error | extra context line |
| 6 | `ls -l` | Different timestamp on `test/` directory | Different timestamp on `test/` directory | run timing only |

---

## SUMMARY TABLE

| Bug | Command | Severity |
|---|---|---|
| Bug 1 | `/bin/echo hello &&' /bin/echo bye` | Medium — wrong output with `&&` + unclosed quote |
| Bug 2 | `/bin/echo "hello` | Medium — unclosed `"` should error |
| Bug 3 | `/bin/echo hel"lo"'` | Medium — unclosed `'` should error |
| Bug 4 | `foo > file1 2> file2` | **High** — `2>` does not redirect command-not-found errors |
