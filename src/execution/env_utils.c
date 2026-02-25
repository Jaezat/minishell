/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_env_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 02:33:42 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/20 02:33:59 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *head, char *env_var)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, env_var) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*get_env_node(t_env *head, char *key)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	update_env_var(t_env *head, char *env_var, char *new_value)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, env_var) == 0)
		{
			if (current->value)
				free(current->value);
			current->value = ft_strdup(new_value);
			return ;
		}
		current = current->next;
	}
}
