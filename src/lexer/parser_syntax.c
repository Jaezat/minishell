#include "minishell.h"

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
