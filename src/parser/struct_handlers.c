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

int	handle_pipe(t_struct *st, t_minishell *data)
{
	st->current_cmd->args[st->arg_index] = NULL;
	st->current_cmd->args = args_expansion(st->current_cmd->args, data);
	st->current_cmd->next = create_node();
	if (!st->current_cmd->next)
		return (0);
	st->current_cmd = st->current_cmd->next;
	st->arg_index = 0;
	if (st->current_tkn->next)
		st->current_cmd->size_args = count_size_args(st->current_tkn->next);
	st->current_cmd->args = ft_calloc
		(st->current_cmd->size_args + 1, sizeof(char *));
	if (!st->current_cmd->args)
		return (0);
	return (1);
}

int	handle_word(t_struct *st)
{
	int		j;
	char	**value_divided;

	value_divided = ft_split_upgrade(st->current_tkn->value, ' ');
	if (!value_divided)
		return (0);
	j = 0;
	while (value_divided[j])
	{
		if (st->arg_index < st->current_cmd->size_args)
			st->current_cmd->args[st->arg_index++] = value_divided[j];
		else
			free(value_divided[j]);
		j++;
	}
	free(value_divided);
	return (1);
}

int	handle_redir(t_struct *st, t_minishell *data)
{
	char	*file;

	if (!st->current_tkn->next)
		return (ft_putstr_fd
			("minishell: syntax error near unexpected token `newline'\n", 2)
			, 0);
	if (st->current_tkn->type == T_REDIR_HDOC)
	{
		file = manage_hdoc(st->current_tkn->next->value, data);
		if (!file)
			return (0);
		add_redir_node(st->current_cmd, T_REDIR_HDOC, file);
		free(file);
	}
	else
	{
		file = check_after_dollar_sign(st->current_tkn->next->value, data);
		if (!file)
			return (0);
		add_redir_node(st->current_cmd, st->current_tkn->type, file);
		free(file);
	}
	st->current_tkn = st->current_tkn->next;
	return (1);
}

/* 
int	handle_redir(t_struct *st, t_minishell *data)
{
	char	*file;

	if (!st->current_tkn->next)
		return (print_error_syntax("newline"));

	if (st->current_tkn->type == T_REDIR_HDOC)
	{
		// PASO CRÍTICO: Usamos el valor del token directamente sin check_after_dollar_sign
		file = manage_hdoc(st->current_tkn->next->value, data);
		if (!file)
			return (0);
		add_redir_node(st->current_cmd, T_REDIR_HDOC, file);
		free(file);
	}
	else
	{
		// Las otras redirecciones SÍ se expanden
		file = check_after_dollar_sign(st->current_tkn->next->value, data);
		// ... resto del código ...
	}
	st->current_tkn = st->current_tkn->next;
	return (1);
} */



int	process_token(t_struct *st, t_minishell *data)
{
	if (st->current_tkn->type == T_PIPE)
		return (handle_pipe(st, data));
	else if (st->current_tkn->type == T_WORD)
		return (handle_word(st));
	else if (st->current_tkn->type >= T_REDIR_IN
		&& st->current_tkn->type <= T_REDIR_HDOC)
		return (handle_redir(st, data));
	return (1);
}
