/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:24:11 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 16:35:43 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**append_one(char **res, int *count, char *str)
{
	char	**new_res;
	int		i;

	new_res = ft_calloc(*count + 2, sizeof(char *));
	if (!new_res)
		return (free(str), res);
	i = -1;
	while (++i < *count)
		new_res[i] = res[i];
	new_res[(*count)++] = str;
	free(res);
	return (new_res);
}

static char	**process_arg(char **result, int *count,
	char *arg, t_minishell *data)
{
	char	*expanded;
	char	**words;
	char	*clean;
	int		i;

	expanded = check_after_dollar_sign(arg, data);
	free(arg);
	words = ft_split_upgrade(expanded, ' ');
	free(expanded);
	if (!words)
		return (result);
	i = 0;
	while (words[i])
	{
		clean = remove_quotes(words[i]);
		free(words[i]);
		if (clean)
			result = append_one(result, count, clean);
		i++;
	}
	free(words);
	return (result);
}

char	**args_expansion(char **args, t_minishell *data)
{
	char	**result;
	int		count;
	int		i;

	if (!args)
		return (NULL);
	result = ft_calloc(1, sizeof(char *));
	count = 0;
	i = 0;
	while (args[i])
	{
		printf("args: %s\n", args[i]);
		result = process_arg(result, &count, args[i], data);
		i++;
	}
	free(args);
	return (result);
}

t_expand	*init_expand(void)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->i = 0;
	exp->single_quote = 0;
	exp->double_quote = 0;
	exp->res = ft_strdup("");
	if (!exp->res)
	{
		free(exp);
		return (NULL);
	}
	return (exp);
}
