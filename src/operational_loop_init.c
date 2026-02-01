#include "minishell.h"


void readline_calling(char **line)
{
    *line = readline(
        "\001\033[1;35m\002minishell$>\001\033[0m\002 "
    );
}

int has_unclosed_quotes(char *str)
{
	int in_single;
	int in_double;
	
	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (*str == '"' && !in_single)
			in_double = !in_double;
		else if (*str == '\'' && !in_double)
			in_single = !in_single;
		str++;
	}
	return (in_single || in_double);
}

char *ft_strjoin_with_newline(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	
	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 2);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	result[len1] = '\n';
	ft_memcpy(result + len1 + 1, s2, len2);
	result[len1 + len2 + 1] = '\0';
	
	return (result);
}

char *read_complete_line()
{
	char	*line;
	char	*continuation;
	char	*temp;
	
	line = readline("minishell$> ");
	if (!line)
		return (NULL);
	while (has_unclosed_quotes(line))
	{
		continuation = readline("> ");
		if (!continuation)
		{
			printf("syntax error: unexpected end of file\n");
			free(line);
			return (NULL);
		}
		temp = line;
		line = ft_strjoin_with_newline(temp, continuation);
		free(temp);
		free(continuation);
		if (!line)
			return (NULL);
	}
	return (line);
}

int is_interactive(t_minishell *data)
{
	if (data->line)
		free(data->line);
	data->line = read_complete_line();
	if (!data->line)
		return (1);
	if (*data->line)
		add_history(data->line);
	
	return (0);
}

/* int start_operational_loop(t_minishell *data)
	current = data->list_tokens;
	while(current)
	{
		if(current->type == T_WORD)
			{
				//printf("WORD: %s\n", current->value);
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
					{
						args[0] = current->next->value;
						args[1] = NULL;
					}
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
 */

/* int start_operational_loop(t_minishell *data)
{
	int result;

	while (1)
	{
		if (is_interactive(data) == 1)
			return (1);
		result = tokenize_input(data);
		if (result == 0)
		{
			// printf("Line: %s\n", data->line);
			if (check_syntax(data->list_tokens) != 0)
			{
				free_token_list(data->list_tokens);
				continue;
			}
			print_tokens(data);
			


		}
	}
	return (0);
} */

int start_operational_loop(t_minishell *data)
{
    t_cmd   *cmds;

    while (1)
    {
        if (is_interactive(data) == 1) 
            break;
        if (tokenize_input(data) == 0)
        {
            if (check_syntax(data->list_tokens) != 0)
            {
                free_token_list(data->list_tokens);
                continue;
            }
			// print_tokens(data);
            cmds = create_struct(data);
            if (cmds == NULL)
            {
                free_token_list(data->list_tokens);
                continue;
            }
			print_commands(cmds);
			// printf("here2\n");
			

            // Aquí vendría la ejecución:
            // execute_cmds(cmds, data);

            // LIMPIEZA FINAL DE LA ITERACIÓN
            /* free_cmd_list(cmds);
            free_token_list(data->list_tokens);
        } */
    	}
	}
    return (0);
}

