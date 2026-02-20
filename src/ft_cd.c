/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 02:24:09 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/20 02:37:34 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (path)
		ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	perror(NULL);
}

static void	update_working_dirs(t_minishell *shell, char *old_cwd)
{
	char	new_cwd[PATH_MAX];

	update_env_var(shell->env_list, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, PATH_MAX) != NULL)
		update_env_var(shell->env_list, "PWD", new_cwd);
}

int	ft_cd(t_minishell *shell, char **args)
{
	char	*target_path;
	char	old_cwd[PATH_MAX];

	if (!args[1])
	{
		target_path = get_env_value(shell->env_list, "HOME");
		if (!target_path)
			return (perror("minishell: cd: HOME not set"), 1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target_path = get_env_value(shell->env_list, "OLDPWD");
		if (!target_path)
			return (perror("minishell: cd: OLDPWD not set"), 1);
		ft_printf("%s\n", target_path);
	}
	else
		target_path = args[1];
	getcwd(old_cwd, PATH_MAX);
	if (chdir(target_path) == -1)
		return (print_cd_error(target_path), 1);
	update_working_dirs(shell, old_cwd);
	return (0);
}
