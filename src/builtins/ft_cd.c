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

	if (old_cwd[0] != '\0')
		update_env_var(shell->env_list, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, PATH_MAX) != NULL)
		update_env_var(shell->env_list, "PWD", new_cwd);
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
	}
}

static void	protect_from_null_cwd(char *old_cwd, t_env *env)
{
	char	*env_pwd;

	env_pwd = get_env_value(env, "PWD");
	if (env_pwd)
		ft_strlcpy(old_cwd, env_pwd, PATH_MAX);
	else
		old_cwd[0] = '\0';
}

static int	set_target_path(char **target_path, char **args, t_env *env)
{
	if (!args[1])
	{
		*target_path = get_env_value(env, "HOME");
		if (!*target_path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (0);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		*target_path = get_env_value(env, "OLDPWD");
		if (!*target_path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (0);
		}
		ft_printf("%s\n", *target_path);
	}
	else
		*target_path = args[1];
	return (1);
}

int	ft_cd(t_minishell *shell, char **args)
{
	char	*target_path;
	char	old_cwd[PATH_MAX];

	if (args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 2);
	if (!set_target_path(&target_path, args, shell->env_list))
		return(1);

	if (getcwd(old_cwd, PATH_MAX) == NULL)
		protect_from_null_cwd(old_cwd, shell->env_list);
	if (chdir(target_path) == -1)
		return (print_cd_error(target_path), 1);
	update_working_dirs(shell, old_cwd);
	return (0);
}
