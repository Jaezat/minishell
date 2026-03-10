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

static void	wait_for_all_children(t_minishell *shell, pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		printed;

	printed = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		print_signal_once(status, &printed);
		if (pid == last_pid)
			shell->exit_status = decode_status(status);
		pid = waitpid(-1, &status, 0);
	}
}

static pid_t	fork_child(t_minishell *shell, t_cmd *cmd, int fd[2], int fd_in)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	else if (pid == 0)
		child_process(shell, cmd, fd, fd_in);
	return (pid);
}

void	execute_pipeline(t_minishell *shell, t_cmd *cmd)
{
	int		fd[2];
	int		fd_in;
	pid_t	pid;
	pid_t	last_pid;

	signal(SIGINT, SIG_IGN);
	last_pid = -1;
	fd_in = -1;
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			return (perror("pipe"));
		pid = fork_child(shell, cmd, fd, fd_in);
		if (pid == -1)
			break ;
		last_pid = pid;
		parent_process(cmd, fd, &fd_in);
		cmd = cmd->next;
	}
	wait_for_all_children(shell, last_pid);
	handle_signals();
}
