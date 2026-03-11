/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:25:11 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/07 13:27:53 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_minishell *shell, t_cmd *cmd)
{
	char	*name;

	name = cmd->args[0];
	if (ft_strcmp(name, "echo") == 0)
		return (ft_echo(cmd->args));
	if (ft_strcmp(name, "cd") == 0)
		return (ft_cd(shell, cmd->args));
	if (ft_strcmp(name, "pwd") == 0)
		return (ft_pwd(shell->env_list, cmd->args));
	if (ft_strcmp(name, "export") == 0)
		return (ft_export(shell, cmd->args));
	if (ft_strcmp(name, "unset") == 0)
		return (ft_unset(shell, cmd->args));
	if (ft_strcmp(name, "env") == 0)
		return (ft_env(shell->env_list, cmd->args));
	if (ft_strcmp(name, "exit") == 0)
		return (ft_exit(shell, cmd->args));
	return (1);
}

int	is_lone_builtin(t_cmd *cmd)
{
	if (cmd->next)
		return (0);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (1);
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (1);
	return (0);
}

void	execute_lone_builtin(t_minishell *shell, t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;

	if (!cmd->redirs)
	{
		shell->exit_status = execute_builtin(shell, cmd);
		return ;
	}
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	handle_redirections(cmd->redirs);
	shell->exit_status = execute_builtin(shell, cmd);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}
