/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 14:27:01 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/08 14:36:03 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_flags_and_target_fd(t_redir *redir, int *flags, int *target_fd)
{
	if (redir->type == T_REDIR_IN)
	{
		*flags = O_RDONLY;
		*target_fd = STDIN_FILENO;
	}
	else
	{
		*target_fd = STDOUT_FILENO;
		if (redir->type == T_REDIR_OUT)
			*flags = O_WRONLY | O_CREAT | O_TRUNC;
		else
			*flags = O_WRONLY | O_CREAT | O_APPEND;
	}
}

void	execute_builtin_and_exit(t_minishell *shell, t_cmd *cmd)
{
	int	exit_code;

	exit_code = execute_builtin(shell, cmd);
	free_all_data(shell);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit(exit_code);
}
