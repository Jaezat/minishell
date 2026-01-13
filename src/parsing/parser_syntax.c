#include "minishell.h"

/* 
while (1)
	{
		if (is_interactive(data) == 1)
			return (1);
		result = tokenize_input(data);		
		if (result == 0)
		{
			print_tokens(data->list_tokens);
			
		}
	}
*/

/* Handling pipe errors */

int print_error_pipe()
{
    printf("Syntax error near unexpected token `|'\n");
    return (1);
}


int has_invalid_pipes(t_token *list_tokens)
{
    t_token *current;

    current = list_tokens;
    while(current)
    {
        if (current == list_tokens)
        {
            if (current->type == T_PIPE)
                return (print_error_pipe());
        }
        else if (current->type == T_PIPE && current->next->type == T_PIPE)
            return (print_error_pipe());
        current = current->next;
    }
    return 0;
}