# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
make          # Build the minishell executable
make re       # Full rebuild (fclean + all)
make clean    # Remove object files
make fclean   # Remove everything including executable
make valgrind # Run with memory leak detection (recommended for leak checks)
make valgrind_full  # Extended valgrind with origin tracking
```

Compiler: `cc` with flags `-Wall -Wextra -Werror -g`. Depends on `libft/` (custom C library) and system `readline`.

## Running Tests

```bash
bash tester.sh   # Compare minishell output against bash
bash tester1.sh  # Extended test suite
bash tester2.sh  # Additional test suite
```

Tests run commands through both bash and minishell and diff the outputs.

## Architecture

The shell processes input through five sequential stages:

```
Input → Tokenizer → Parser → Expander → Executor
```

**1. Tokenizer (`src/lexer/`)** — converts raw input into a `t_token` linked list. Token types: `T_WORD`, `T_PIPE`, `T_REDIR_IN`, `T_REDIR_OUT`, `T_REDIR_APPEND`, `T_REDIR_HDOC`, `T_EOF`, `T_ERROR`. Syntax validation happens here via `check_syntax()`.

**2. Parser (`src/parser/`)** — converts token list into a `t_cmd` linked list (one node per pipeline stage). Each `t_cmd` has a `char **args` array and a `t_redir` linked list. Quotes are stripped here.

**3. Expander (`src/expansion/`)** — walks each argument and expands `$VAR`, `$?`, and handles heredoc delimiters. Single quotes suppress expansion; double quotes allow `$` expansion.

**4. Executor (`src/execution/`)** — dispatches to built-ins or forks external commands. Pipelines use `fork()` + `pipe()` + `dup2()`. Redirections are applied in the child before `execve()`. Exit status is captured from the last pipeline command.

**5. Built-ins (`src/builtins/`)** — `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`. Built-ins in a solo command run in the parent process (to affect shell state); built-ins inside a pipeline run in a fork.

## Key Data Structures (`include/`)

```c
t_minishell   // Global shell state: env list, current line, exit status, token list, quote state
t_env         // Linked list of environment variables: key, value, is_exported
t_token       // Linked list of lexed tokens: type, value
t_cmd         // Linked list of parsed commands: args array, redirs list
t_redir       // Linked list of redirections: type, filename/delimiter
t_expand      // State for variable expansion: result string, position, quote flags
t_struct      // Parser working state: head/current cmd, current token, arg index
```

## Important Details

- **Signal handling**: Global `g_signal_status` tracks `SIGINT`. Children reset to `SIG_DFL`; the parent ignores signals during pipeline execution. Defined in `src/signals/signals.c`.
- **Non-interactive mode**: When stdin is not a TTY, `get_next_line()` replaces `readline()` and no prompt is shown. Detected in `src/operational_loop_init.c`.
- **Heredocs**: Written to a temporary file during expansion (`src/expansion/struct_hdoc.c`). Quoted delimiters suppress expansion inside the heredoc body.
- **Multiline input**: Unclosed quotes cause the shell to prompt for continuation; tracked via `t_minishell.unclosed_quotes`.
- **Memory**: All structures have corresponding free functions. Run `make valgrind` to check leaks; readline has known internal leaks that are suppressed.
- **libft**: Custom C library in `libft/src/` — do not confuse its functions with standard library counterparts.
