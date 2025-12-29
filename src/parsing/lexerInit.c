#include "minishell.h"
 
t_token *create_token(t_token_type type, char *value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->type = type;
	if(value)
	{
		token->value = ft_strdup(value);
		if(!token->value)
		{
			free(token);
			return (NULL);
		}
	}
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

void skip_space(char c, int *i)
{
	if(c == ' ' || c == '\t')
		(*i)++;
}

void free_token_list(t_token *head)
{
	t_token *temp;

	while(head != NULL)
	{
		temp = head->next;
		free(head->value);
		free(head);
		head = temp;
	}
}

int check_validation_token(char **str, t_token *head, t_token *token)
{
	if(!token)
	{
		free(*str);
		free_token_list(head);
		return (1);
	}
	return (0);
}

int is_index_space_or_operator(char c)
{
	if(c == ' ' || c == '\t')
		return (1);
	else if(c == '>' || c == '<' || c == '|')
		return (1);
	return 0;
}

char *extract_word(char *str, int *i)
{
	char *word;
	int start;
	int inside_double; 
	int inside_single;

	start = *i;
	inside_double = 0;
	inside_single = 0;
	while(str[*i])
	{
		if(str[*i] == '"' && !inside_single)
			inside_double = !inside_double;
		else if(str[*i] == '\'' && !inside_double)
			inside_single = !inside_single;
		else if(!inside_double && !inside_single && is_index_space_or_operator(str[*i]))
			break;
		(*i)++;
	}
	if (inside_double || inside_single)
	{
		printf("Syntax error: unclosed quotes\n"); // question here: should we imitate bash or end program
		return (NULL);
	}
	word = malloc(sizeof(char) * (*i - start + 1));
	if (!word)
		return (NULL);
	ft_memcpy(word, &str[start], *i - start);
	word[*i - start] = '\0';
	return (word);
}

void add_token(t_token **head, t_token **tail, t_token *token)
{
	if (!*head)
	{
		*head = token;
		*tail = token;
	}
	else
	{
		(*tail)->next = token;
		*tail = token;
	}
}    

// detect operators
t_token	*check_operator(char *str, int *i)
{
	if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*i += 2;
		return (create_token(T_REDIR_APPEND, ">>"));
	}
	if (str[*i] == '<' && str[*i + 1] == '<')
	{
		*i += 2;
		return (create_token(T_REDIR_HDOC, "<<"));
	}
	if (str[*i] == '>')
	{
		(*i)++;
		return (create_token(T_REDIR_OUT, ">"));
	}
	if (str[*i] == '<')
	{
		(*i)++;
		return (create_token(T_REDIR_IN, "<"));
	}
	if (str[*i] == '|')
	{
		(*i)++;
		return (create_token(T_PIPE,  "|"));
	}
	return (NULL);
}

int	tokenize_input(t_data *data)
{
	int		i;
	char	*word;  
	t_token	*token;
	t_token	*head;  
	t_token	*tail; 
    
	i = 0;   
	head = NULL;  
	tail = NULL;   
	while (data->line[i])  
	{
		skip_space(data->line[i], &i);
		if (!data->line[i])
			break;
		token = check_operator(data->line, &i);
		if (!token)
		{
			word = extract_word(data->line, &i);
			if (!word)
				return (1);
			token = create_token(T_WORD, word);
			free(word);
		}
		if (check_validation_token(&data->line, head, token))
			return (1);
		add_token(&head, &tail, token);
	}
	data->list_tokens = head;
	return (0); 
}