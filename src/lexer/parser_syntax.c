#include "minishell.h"

int	print_error_syntax(char *str)
{
	char	*err_msg;

	err_msg = "minishell: syntax error near unexpected token ";
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

/* 
OG function
int	has_invalid_pipes(t_token *list_tokens)
{
	t_token	*current;
	char	*err_pipe;

	current = list_tokens;
	err_pipe = "|";
	while (current != NULL)
	{
		if (current == list_tokens)
		{
			if (current->type == T_PIPE)
				return (print_error_syntax(err_pipe));
		}
		else if (current->next != NULL)
		{
			if (current->type == T_PIPE && current->next->type == T_PIPE)
				return (print_error_syntax(err_pipe));
		}
		else if (current->next == NULL && current->type == T_PIPE)
			return (print_error_syntax(err_pipe));
		current = current->next;
	}
	return (0);
} */

int has_invalid_pipes(t_token *list_tokens)
{
    t_token *current = list_tokens;

    while (current != NULL)
    {
        if (current == list_tokens && current->type == T_PIPE)
            return (print_error_syntax("|"));

        if (current->type == T_PIPE)
        {
            if (current->next == NULL)
                return (print_error_syntax("newline"));
            if (current->next->type != T_WORD) 
                return (print_error_syntax(current->next->value));
        }
        current = current->next;
    }
    return (0);
}

int	check_redirec(t_token *token)
{
	if (token->type != T_REDIR_IN && token->type != T_REDIR_OUT
		&& token->type != T_REDIR_APPEND && token->type != T_REDIR_HDOC)
		return (0);
	if (token->next == NULL)
		return (print_error_syntax("newline"));
	if (token->next->type != T_WORD)
		return (print_error_syntax(token->next->value));
	return (0);
}

int	has_invalid_redirect(t_token *list_tokens)
{
	t_token	*current;
	int		status;

	current = list_tokens;
	while (current != NULL)
	{
		status = check_redirec(current);
		if (status != 0)
			return (status);
		current = current->next;
	}
	return (0);
}

int	check_syntax(t_token *list_tokens)
{
	t_token *tmp = list_tokens;

	if (tmp && tmp->type == T_ERROR)
        return (print_error_syntax(tmp->value));
	if (has_invalid_pipes(list_tokens) != 0)
		return (1);
	if (has_invalid_redirect(list_tokens) != 0)
		return (1);
	return (0);
}
