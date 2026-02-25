/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:44:28 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/25 15:45:29 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_key_len(char *arg, char *equal_sign)
{
	if (equal_sign)
		return (equal_sign - arg);
	return (ft_strlen(arg));
}

int	validate_shell_variable(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_env	*create_env_from_key_and_value(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->is_exported = 1;
	node->next = NULL;
	return (node);
}

void	extract_key_and_value_from_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(arg, '=');
	key_len = get_key_len(arg, equal_sign);
	*key = ft_substr(arg, 0, key_len);
	if (equal_sign)
		*value = ft_strdup(arg + key_len + 1);
	else
		*value = NULL;
}

void	upsert_env_var(t_minishell *shell, char *arg)
{
	t_env	*node;
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	extract_key_and_value_from_arg(arg, &key, &value);
	node = get_env_node(shell->env_list, key);
	if (node)
	{
		if (equal_sign)
		{
			free(node->value);
			node->value = value;
		}
		else
			free(value);
		free(key);
	}
	else
	{
		node = create_env_from_key_and_value(key, value);
		add_env_node(&shell->env_list, node);
	}
}
