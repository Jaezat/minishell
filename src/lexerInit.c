#include "minishell.h"
 
t_token *createToken(t_token_type type, char *value)
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

void skipSpace(char c, int *i)
{
	if(c == ' ' || c == '\t')
		(*i)++;
}

void freeTokenList(t_token *head)
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

int checkValidationToken(char **str, t_token *head, t_token *token)
{
	if(!token)
	{
		free(*str);
		freeTokenList(head);
		return (1);
	}
	return (0);
}

int isIndexSpaceOrOperator(char c)
{
	if(c == ' ' || c == '\t')
		return (1);
	else if(c == '>' || c == '<' || c == '|')
		return (1);
	return 0;
}

char *extractWord(char *str, int *i)
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
		else if(!inside_double && !inside_single && isIndexSpaceOrOperator(str[*i]))
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

void addToken(t_token **head, t_token **tail, t_token *token)
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
t_token	*checkOperator(char *str, int *i)
{
	if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*i += 2;
		return (createToken(T_REDIR_APPEND, ">>"));
	}
	if (str[*i] == '<' && str[*i + 1] == '<')
	{
		*i += 2;
		return (createToken(T_REDIR_HDOC, "<<"));
	}
	if (str[*i] == '>')
	{
		(*i)++;
		return (createToken(T_REDIR_OUT, ">"));
	}
	if (str[*i] == '<')
	{
		(*i)++;
		return (createToken(T_REDIR_IN, "<"));
	}
	if (str[*i] == '|')
	{
		(*i)++;
		return (createToken(T_PIPE,  "|"));
	}
	return (NULL);
}

int	tokenizeInput(t_data *data)
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
		skipSpace(data->line[i], &i);
		if (!data->line[i])
			break;
		token = checkOperator(data->line, &i);
		if (!token)
		{
			word = extractWord(data->line, &i);
			if (!word)
				return (1);
			token = createToken(T_WORD, word);
			free(word);
		}
		if (checkValidationToken(&data->line, head, token))
			return (1);
		addToken(&head, &tail, token);
	}
	data->list_tokens = head;
	return (0); 
}