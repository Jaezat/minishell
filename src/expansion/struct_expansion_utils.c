/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_expansion_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:24:05 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 10:47:05 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (ft_substr(str, 0, len));
}

char	*get_env_value_struct(char *name, t_minishell *data)
{
	t_env	*current;
	char	*value;

	value = NULL;
	current = data->env_list;
	while (current != NULL)
	{
		if (ft_strcmp(name, current->key) == 0)
		{
			value = current->value;
			return (value);
		}
		current = current->next;
	}
	return (NULL);
}

char	*join_char(char *res, char c)
{
	char	*new_str;
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	if (!res)
		return (ft_strdup(temp));
	new_str = ft_strjoin(res, temp);
	free(res);
	return (new_str);
}

char	*expand_exit_status(char *res, t_minishell *data)
{
	char	*value;
	char	*tmp;

	value = ft_itoa(data->exit_status);
	tmp = res;
	res = ft_strjoin(tmp, value);
	free(tmp);
	free(value);
	return (res);
}

char	*expand_variable(char *res, char *str, int *i, t_minishell *data)
{
	char	*var_name;
	char	*value;
	char	*tmp;

	var_name = get_var_name(str);
	value = get_env_value_struct(var_name, data);
	if (value)
	{
		tmp = res;
		res = ft_strjoin(tmp, value);
		free(tmp);
	}
	*i += ft_strlen(var_name);
	free(var_name);
	return (res);
}
