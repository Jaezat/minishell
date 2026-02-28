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

void	execute_commands(t_minishell *shell, t_cmd *cmd)
{
	if (handle_heredocs(cmd, shell) == 1)
	{
		shell->exit_status = 130;
		return ;
	}
	if (is_lone_builtin(cmd))
		execute_lone_builtin(shell, cmd);
	else
		execute_pipeline(shell, cmd);
}
