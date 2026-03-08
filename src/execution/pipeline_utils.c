/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:37:50 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/08 14:40:51 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	list_len(t_env *list)
{
	size_t	i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

static char	**get_env_array(t_env *env_list)
{
	char	**env_array;
	char	*tmp;
	int		i;

	env_array = malloc(sizeof(char *) * (list_len(env_list) + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env_list)
	{
		if (env_list->value)
		{
			tmp = ft_strjoin(env_list->key, "=");
			env_array[i] = ft_strjoin(tmp, env_list->value);
			free(tmp);
			if (!env_array[i])
				return (free_2d_array(env_array), NULL);
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	handle_redirections(t_redir *redir)
{
	int	fd;
	int	flags;
	int	target_fd;

	while (redir)
	{
		set_flags_and_target_fd(redir, &flags, &target_fd);
		fd = open(redir->file, flags, 0644);
		if (check_fd_error(fd, redir->file) == -1)
			return (-1);
		dup2(fd, target_fd);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

void	handle_child_pipes(t_cmd *cmd, int *fd, int fd_in)
{
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
}

void	run_execution(t_minishell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;

	if (!cmd->args[0])
	{
		free_all_data(shell);
		exit(0);
	}
	if (is_builtin(cmd->args[0]))
		execute_builtin_and_exit(shell, cmd);
	path = get_cmd_path(shell, cmd->args[0]);
	env = get_env_array(shell->env_list);
	if (!path)
	{
		print_path_error(cmd->args[0]);
		free_2d_array(env);
		free_all_data(shell);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("execve");
	free(path);
	free_2d_array(env);
	free_all_data(shell);
	exit(1);
}
