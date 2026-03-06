#include "minishell.h"

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

int	check_syntax(t_token *list_tokens)
{
	t_token *current = list_tokens;
	int		status;

	while(current != NULL)
	{
		if (current->type == T_ERROR)
    	    return (print_error_syntax(current->value));
		if (current->type == T_PIPE)
        {
			if(current == list_tokens)
				return (print_error_syntax("|"));
            if (current->next == NULL)
                return (print_error_syntax("newline"));
            if (current->next->type != T_WORD) 
                return (print_error_syntax(current->next->value));
        }
		status = check_redirec(current);
		if (status != 0)
			return (status);
		current = current->next;
	}
	return (0);
}
