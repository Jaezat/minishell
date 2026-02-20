#include "minishell.h"

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
