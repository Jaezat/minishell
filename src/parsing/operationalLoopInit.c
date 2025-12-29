#include "minishell.h"


void readline_calling(char **line)
{
    *line = readline(
        "\001\033[1;35m\002minishell$>\001\033[0m\002 "
    );
}

int is_interactive(t_data *data)
{
    readline_calling(&(data->line));
    if (data->line == NULL)
    {
        ft_printf("Exit");
        return 1;
    }
    if (data->line)
        add_history(data->line);
    return (0);
}

void print_tokens(t_token *head)
{
	t_token *current;

	current = head;
	while(current)
	{
		if(current->type == T_WORD)
			printf("WORD: %s\n", current->value);
		else if(current->type == T_PIPE)
			printf("PIPE: %s\n", current->value);
		else if(current->type == T_REDIR_IN)
			printf("REDIR_IN: %s\n", current->value);
		else if(current->type == T_REDIR_OUT)
			printf("REDIR_OUT: %s\n", current->value);
		else if(current->type == T_REDIR_HDOC)
			printf("REDIR_HDOC: %s\n", current->value);
		else if(current->type == T_REDIR_APPEND)
			printf("REDIR_APPEND: %s\n", current->value);
		else
			printf("EOF: %s\n", current->value);
		current = current->next;
	}
}

int start_operational_loop(t_data *data)
{
    while (1)
    {
        if (is_interactive(data) == 1)
            return 1;
		if (tokenize_input(data))
		{
    	    free_all_data(data);
    	    return 1;
		}
		print_tokens(data->list_tokens);
	}
    return 0;
}