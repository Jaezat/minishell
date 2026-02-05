#include "minishell.h"

t_cmd *create_node(void)
{
	t_cmd *new_cmd;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if(!new_cmd)
		return NULL;
	return (new_cmd);
}

t_struct *init_struct(t_minishell *data)
{
    t_struct *st;

    st = malloc(sizeof(t_struct));
    if (!st)
        return (NULL);
    st->head_cmd = create_node();
    if (!st->head_cmd)
    {
        free(st);
        return (NULL);
    }
    st->current_cmd = st->head_cmd;
    st->current_tkn = data->list_tokens;
    st->arg_index = 0;
    st->current_cmd->size_args = count_size_args(st->current_tkn);
    st->current_cmd->args = malloc(sizeof(char *) 
        * (st->current_cmd->size_args + 1));
    if (!st->current_cmd->args)
    {
        free(st->head_cmd);
        free(st);
        return (NULL);
    }
    return (st);
}

t_cmd *create_struct(t_minishell *data)
{
    t_struct *st;
    t_cmd *result;

    if (!data || !data->list_tokens)
        return (NULL);
    st = init_struct(data);
    if (!st)
        return (NULL);
    while (st->current_tkn)
    {
        if (!process_token(st, data))
        {
            free(st);
            return (NULL);
        }
        st->current_tkn = st->current_tkn->next;
    }
    st->current_cmd->args[st->arg_index] = NULL;
    st->current_cmd->args = args_expansion(st->current_cmd->args, data);
    result = st->head_cmd;
    free(st);
    return (result);
}