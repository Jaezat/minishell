/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:37:41 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/07 13:38:21 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_minishell *shell, t_cmd *cmd, int *fd, int fd_in)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_child_pipes(cmd, fd, fd_in);
	if (handle_redirections(cmd->redirs) == -1)
	{
		free_all_data(shell);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		exit(1);
	}
	run_execution(shell, cmd);
}

static void	parent_process(t_cmd *cmd, int *fd, int *fd_in)
{
	if (*fd_in != -1)
		close(*fd_in);
	if (cmd->next)
	{
		close(fd[1]);
		*fd_in = fd[0];
	}
}

static void	wait_for_all_children(t_minishell *shell)
{
	int	status;
	int	sig_newline_printed;

	sig_newline_printed = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT && !sig_newline_printed)
		{
			write(1, "\n", 1);
			sig_newline_printed = 1;
		}
		else if (WTERMSIG(status) == SIGQUIT && !sig_newline_printed)
		{
			write(1, "Quit (core dumped)\n", 19);
			sig_newline_printed = 1;
		}
		}
	}
}

void	execute_pipeline(t_minishell *shell, t_cmd *cmd)
{
	int		fd[2];
	pid_t	pid;
	int		fd_in;

	fd_in = -1;
	signal(SIGINT, SIG_IGN);
	while (cmd)
	{
		if (cmd->next)
			if (pipe(fd) == -1)
			{
				perror("pipe");
				return;
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
	handle_signals();
}
