/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:28:47 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/10 16:57:14 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_path_env_shell(char *path, char **env, t_minishell *shell)
{
	free(path);
	free_2d_array(env);
	free_all_data(shell);
}

void	clean_and_exit(char *path, char **env, t_minishell *shell, int status)
{
	free_path_env_shell(path, env, shell);
	exit(status);
}

int	get_error_nb(int error)
{
	if (error == ENOENT)
		return (127);
	return (126);
}
