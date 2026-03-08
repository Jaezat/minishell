/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:46:36 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/25 15:46:43 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*env_pwd;

	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}

	env_pwd = get_env_value(env, "PWD");
	if (env_pwd)
	{
		printf("%s\n", env_pwd);
		return (0);
	}

	perror("minishell: pwd");
	return (1);
}
