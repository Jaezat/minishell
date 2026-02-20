/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:26:31 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/20 03:33:14 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_lone_builtin(t_cmd *cmd)
{
	if (cmd->next)
		return (0);

	if (ft_strcmp(cmd->args[0], "cd") == 0) return (1);
	if (ft_strcmp(cmd->args[0], "exit") == 0) return (1);
	if (ft_strcmp(cmd->args[0], "export") == 0) return (1);
	if (ft_strcmp(cmd->args[0], "unset") == 0) return (1);
	return (0);
}

static void	execute_lone_builtin(t_minishell *shell, t_cmd *cmd)
{
	int original_stdin;
	int original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	handle_redirections(cmd->redirs);
	shell->exit_status = execute_builtin(shell, cmd);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

void	execute_commands(t_minishell *shell, t_cmd *cmd)
{
	if (handle_heredocs(cmd) == 1)
	{
		shell->exit_status = 130;
		return ;
	}
	if (is_lone_builtin(cmd))
		execute_lone_builtin(shell, cmd);
	else
		execute_pipeline(shell, cmd);
}
