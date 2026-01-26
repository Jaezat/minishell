#include "minishell.h"

void print_tokens(t_minishell *data)
{
	t_token *current;

	current = data->list_tokens;
	while(current)
	{
		if(current->type == T_WORD)
			{
				printf("WORD: %s\n", current->value);
				if ((ft_strcmp(current->value, "cd") == 0))
				{
					char *arg;
					if (!(current->next))
						arg = NULL;
					else
						arg = current->next->value;
					ft_cd(data, arg);
				}
				if ((ft_strcmp(current->value, "env") == 0))
					print_env_list(data->env_list);
				if ((ft_strcmp(current->value, "pwd") == 0))
					ft_pwd();
				if ((ft_strcmp(current->value, "exit") == 0))
				{
					char *args[2];
					if (!(current->next))
						args[0] = NULL;
					else
						args[0] = current->next->value;
					if (current->next && current->next->next)
						args[1] = current->next->next->value;
					ft_exit(data, args);
				}
				if ((ft_strcmp(current->value, "echo") == 0))
				{
					if (current->next)
						current = current->next;
					
					ft_echo(data, current);
				}
			}
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
