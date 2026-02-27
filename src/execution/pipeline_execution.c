#include "minishell.h"

static void child_process(t_minishell *shell, t_cmd *cmd, int *fd, int fd_in)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_child_pipes(cmd, fd, fd_in);
	handle_redirections(cmd->redirs);
	run_execution(shell, cmd);
}

static void parent_process(t_cmd *cmd, int *fd, int *fd_in)
{
	if (*fd_in != -1)
		close(*fd_in);
	if (cmd->next)
	{
		close(fd[1]);
		*fd_in = fd[0];
	}
}

static void wait_for_all_children(t_minishell *shell)
{
	int status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}
}

void	execute_pipeline(t_minishell *shell, t_cmd *cmd)
{
	int		fd[2];
	pid_t	pid;
	int		fd_in;

	fd_in = -1;
	while (cmd)
	{
		if (cmd->next)
			if (pipe(fd) == -1)
			{
				perror("pipe");
				return; //shouldn't this be an exit?
			}
		pid = fork();
		if (pid == -1)
			perror("fork");
		else if (pid == 0)
			child_process(shell, cmd, fd, fd_in);
		parent_process(cmd, fd, &fd_in);
		cmd = cmd->next;
	}
	wait_for_all_children(shell);
}
