/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:17:02 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/11 14:58:58 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_tilde(char *path, t_env *env, int *is_alloc)
{
	char	*home;
	char	*str_without_tilde;
	char	*final_path;

	home = get_env_value(env, "HOME");
	if (!home)
		return (path);
	str_without_tilde = ft_strdup(path + 1);
	final_path = ft_strjoin(home, str_without_tilde);
	free(str_without_tilde);
	*is_alloc = 1;
	return (final_path);
}

int	is_builtin_flag(char *arg)
{
	if (!arg || !arg[0])
		return (0);
	if ((arg[0] == '-') && arg[1])
		return (1);
	return (0);
}

void	print_bt_flag_error(char *builtin, char *flag)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(builtin, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(flag, 2);
	ft_putstr_fd(": invalid option \n", 2);
}
