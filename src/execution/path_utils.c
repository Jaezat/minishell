/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:36:01 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/10 16:48:05 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_full_path(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (1);
	return (0);
}

static char	*build_full_path(char *folder, char *cmd)
{
	char	*full_path;
	char	*tmp;

	tmp = ft_strjoin(folder, "/");
	full_path = ft_strjoin(tmp, cmd);
	free (tmp);
	return (full_path);
}

int	is_valid_absolute_path(char *cmd)
{
	if (!is_full_path(cmd))
		return (0);
	if (access(cmd, F_OK) == 0)
		return (1);
	return (0);
}

char	*get_cmd_path(t_minishell *shell, char *cmd)
{
	char	*env_path;
	char	**folders;
	char	**head;
	char	*full_path;

	if (is_full_path(cmd))
		return (ft_strdup(cmd));
	env_path = get_env_value(shell->env_list, "PATH");
	if (!env_path)
		return (NULL);
	folders = ft_split(env_path, ':');
	head = folders;
	while (*folders)
	{
		full_path = build_full_path(*folders, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_2d_array(head);
			return (full_path);
		}
		free(full_path);
		folders++;
	}
	free_2d_array(head);
	return (NULL);
}
