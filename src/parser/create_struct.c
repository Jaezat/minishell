/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_init_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:25:13 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 09:25:14 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_size_args(t_token *tkn)
{
	int		count;
	char	**str;
	t_token	*current;

	count = 0;
	current = tkn;
	while (current && current->type != T_PIPE)
	{
		if (current->type == T_WORD)
		{
			str = ft_split_upgrade(current->value, ' ');
			count += ft_size_2d_array(str);
			free_2d_array(str);
		}
		else if (current->type >= T_REDIR_IN && current->type <= T_REDIR_HDOC)
		{
			if (current->next)
				current = current->next;
		}
		current = current->next;
	}
	return (count);
}

static int	process_loop(t_struct *st, t_minishell *data)
{
	while (st->current_tkn)
	{
		if (!process_token(st, data))
			return (0);
		st->current_tkn = st->current_tkn->next;
	}
	return (1);
}

t_cmd	*create_struct(t_minishell *data)
{
	t_struct	*st;
	t_cmd		*result;

	if (!data || !data->list_tokens)
		return (NULL);
	st = init_struct(data);
	if (!st)
		return (NULL);
	if (!process_loop(st, data))
		return (free_struct_assistant(st, 1), NULL);
	st->current_cmd->args[st->arg_index] = NULL;
	st->current_cmd->args = args_expansion(st->current_cmd->args, data);
	if (!st->current_cmd->args)
		return (free_struct_assistant(st, 1), NULL);
	result = st->head_cmd;
	free_struct_assistant(st, 0);
	return (result);
}
