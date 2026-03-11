/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 02:38:30 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/28 15:32:53 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	**duplicate_env_list(t_env *env)
{
	t_env	*current;
	t_env	**env_duplicate;
	int		i;

	env_duplicate = malloc(sizeof(t_env *) * (list_len(env) + 1));
	if (!env_duplicate)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		env_duplicate[i] = current;
		current = current->next;
		i++;
	}
	env_duplicate[i] = NULL;
	return (env_duplicate);
}

static void	print_env_duplicate(t_env **env_duplicate)
{
	int	i;

	i = 0;
	while (env_duplicate[i])
	{
		if (env_duplicate[i]->is_exported)
		{
			printf("declare -x %s", env_duplicate[i]->key);
			if (env_duplicate[i]->value)
				printf("=\"%s\"", env_duplicate[i]->value);
			printf("\n");
		}
		i++;
	}
}

static void	order_env_duplicate(t_env **env_duplicate)
{
	t_env	*tmp;
	int		i;
	int		j;
	int		len;

	len = 0;
	while (env_duplicate[len])
		len++;
	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(env_duplicate[j]->key, env_duplicate[j + 1]->key) > 0)
			{
				tmp = env_duplicate[j];
				env_duplicate[j] = env_duplicate[j + 1];
				env_duplicate[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	env_duplication_and_printing(t_env *env_list)
{
	t_env	**env_duplicate;

	env_duplicate = duplicate_env_list(env_list);
	order_env_duplicate(env_duplicate);
	print_env_duplicate(env_duplicate);
	free(env_duplicate);
}

int	ft_export(t_minishell *shell, char **args)
{
	int		i;
	int		status;

	if (args[0] && args[1] && is_builtin_flag(args[1]))
		return (print_bt_flag_error("env", args[1]), 2);
	status = 0;
	if (!args[1])
		return (env_duplication_and_printing(shell->env_list), 0);
	i = 1;
	while (args[i])
	{
		if (!validate_shell_variable(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			upsert_env_var(shell, args[i]);
		i++;
	}
	return (status);
}
