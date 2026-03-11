/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:17:02 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/11 11:43:59 by andcardo         ###   ########.fr       */
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
