# Execution Changes : Leak Fixes

## Context
When a child process (after `fork`) calls `exit()` without freeing memory,
valgrind reports those allocations as leaks from the child PID.
All changes below ensure the child calls `free_all_data(shell)` before every `exit()`.

---

## 1. `include/execution.h`
Updated two function signatures: `void` → `int`

```c
// BEFORE
void	handle_redirections(t_redir *redir);
void	check_fd_error(int fd, char *filename);

// AFTER
int		handle_redirections(t_redir *redir);
int		check_fd_error(int fd, char *filename);
```

---

## 2. `src/execution/error_utils.c`
`check_fd_error` now returns -1 on error instead of calling `exit(1)`.
This lets the caller decide what to do (including freeing memory first).

```c
// BEFORE
void check_fd_error(int fd, char *filename)
{
    if (fd == -1)
    {
        perror(filename);
        exit(1);   // ← modif
    }
}

// AFTER
int check_fd_error(int fd, char *filename)
{
    if (fd == -1)
    {
        perror(filename);
        return (-1);   // ← caller handles the exit
    }
    return (0);
}
```

---

## 3. `src/execution/pipeline_utils.c`

### `handle_redirections` — propagates the error upward
```c
// BEFORE
void handle_redirections(t_redir *redir)
{
    ...
    fd = open(redir->file, flags, 0644);
    check_fd_error(fd, redir->file);   // ← modif
    dup2(fd, target_fd);
    close(fd);
    ...
}

// AFTER
int handle_redirections(t_redir *redir)
{
    ...
    fd = open(redir->file, flags, 0644);
    if (check_fd_error(fd, redir->file) == -1)
        return (-1);   // ← propagates error, skips dup2/close on bad fd
    dup2(fd, target_fd);
    close(fd);
    ...
    return (0);
}
```

### `run_execution` — frees before every exit in the child
```c
// BEFORE
void run_execution(t_minishell *shell, t_cmd *cmd)
{
    if (is_builtin(cmd->args[0]))
        exit(execute_builtin(shell, cmd));   // modif

    path = get_cmd_path(shell, cmd->args[0]);
    env  = get_env_array(shell->env_list);
    if (!path)
    {
        print_path_error(cmd->args[0]);
        free_2d_array(env);
        exit(127);   // modif
    }
    execve(path, cmd->args, env);
    perror("execve");
    exit(1);   // modif
}

// AFTER
void run_execution(t_minishell *shell, t_cmd *cmd)
{
    if (is_builtin(cmd->args[0]))
    {
        exit_code = execute_builtin(shell, cmd);
        free_all_data(shell);   // ← cleanup before exit
        exit(exit_code);
    }
    path = get_cmd_path(shell, cmd->args[0]);
    env  = get_env_array(shell->env_list);
    if (!path)
    {
        print_path_error(cmd->args[0]);
        free_2d_array(env);
        free_all_data(shell);   // ← cleanup before exit
        exit(127);
    }
    execve(path, cmd->args, env);
    perror("execve");
    free(path);             // ← execve failed, free what was allocated
    free_2d_array(env);
    free_all_data(shell);
    exit(1);
}
```

---

## 4. `src/execution/pipeline_execution.c`

### `child_process` — handles redirection failure with cleanup
```c
// BEFORE
static void child_process(t_minishell *shell, t_cmd *cmd, int *fd, int fd_in)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    handle_child_pipes(cmd, fd, fd_in);
    handle_redirections(cmd->redirs);   // modif
    run_execution(shell, cmd);
}

// AFTER
static void child_process(t_minishell *shell, t_cmd *cmd, int *fd, int fd_in)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    handle_child_pipes(cmd, fd, fd_in);
    if (handle_redirections(cmd->redirs) == -1)   // ← check for error
    {
        free_all_data(shell);   // ← cleanup child's copy of data
        exit(1);
    }
    run_execution(shell, cmd);
}
```

---

## Summary of exit() paths in child process

| Situation | Exit path | Cleaned up? |
|-----------|-----------|-------------|
| Redirection file not found | `child_process` → `exit(1)` | ✅ After fix |
| Builtin inside pipe | `run_execution` → `exit(code)` | ✅ After fix |
| Command not found | `run_execution` → `exit(127)` | ✅ After fix |
| `execve` fails | `run_execution` → `exit(1)` | ✅ After fix |
| `execve` succeeds | process replaced by new program | ✅ No leak possible |
