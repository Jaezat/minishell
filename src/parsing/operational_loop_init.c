#include "minishell.h"


void readline_calling(char **line)
{
    *line = readline(
        "\001\033[1;35m\002minishell$>\001\033[0m\002 "
    );
}

/* int is_interactive(t_data *data)
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
} */

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
	return (in_single || in_double); // check if norminette allows this 
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
	result = malloc(len1 + len2 + 2);  // +1 para '\n' y +1 para '\0'
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

int is_interactive(t_data *data)
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
	int result;
	while (1)
	{
		if (is_interactive(data) == 1)
			return (1);
		result = tokenize_input(data);		
		if (result == 0)
		{
			/* aca comenzamos a trabajar con el tree */
			print_tokens(data->list_tokens);
			if (!has_invalid_pipes(data->list_tokens))
				return (1);
		}
	}
	return (0);
}

/* int start_operational_loop(t_data *data)
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
} */