/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 02:48:16 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/20 02:49:27 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_node_from_list(t_minishell *shell, t_env *target_node)
{
	t_env	**head;

	head = &shell->env_list;
	while (*head && *head != target_node)
		head = &(*head)->next;
	if (*head)
	{
		*head = target_node->next;
		free(target_node->key);
		if (target_node->value)
			free(target_node->value);
		free(target_node);
	}
}

static void	remove_env_var(t_minishell *shell, char *arg)
{
	t_env	*node;
	char	*key;
	char	*value;

	extract_key_and_value_from_arg(arg, &key, &value);
	node = get_env_node(shell->env_list, key);
	if (node)
		remove_node_from_list(shell, node);
	free(key);
	free(value);
}

int	ft_unset(t_minishell *shell, char **args)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (args[i])
	{
		if (!validate_shell_variable(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			remove_env_var(shell, args[i]);
		i++;
	}
	return (status);
}
