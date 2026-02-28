/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:26:31 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/28 18:44:41 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	there_are_commands(t_cmd *cmd)
{
	if (cmd->args[0])
		return (1);
	return (0);
}

void	execute_commands(t_minishell *shell, t_cmd *cmd)
{
	if (handle_heredocs(cmd, shell) == 1)
	{
		shell->exit_status = 130;
		return ;
	}
	if (there_are_commands(cmd) && is_lone_builtin(cmd))
		execute_lone_builtin(shell, cmd);
	else
		execute_pipeline(shell, cmd);
}
