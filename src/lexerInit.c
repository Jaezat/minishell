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
		*i++;
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
	free(temp);
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
 
char *extractWord(char *str, int *i) // is here 
{
	int length;
	char *word;
	int j;

	length = *i;
	while(str[length] && !isIndexSpaceOrOperator(str[length]))
		length++;
	length -= *i;
	word = malloc((sizeof(char) * length) + 1);
	if(!word)
		return (NULL);
	j = 0;
	while(j < length)
	{
		word[j] = str[*i];
		j++;
		*i++;
	}
	word[j] = '\0';
	return (word);
}

int tokenizeInput(t_data *data) //arreglar esta parte
{
	int i;
	char *str;
	char *word;
	t_token *token;
	t_token *head;
	t_token *tail;

	i = 0;
	str = ft_strdup(data->line);
	head = NULL;
	tail = NULL;
	while(str[i])
	{
		skipSpace(str[i], i);
		if(str[i] == '>' && str[i+1] == '>')
		{
			token = createToken(REDIR_APPEND, ">>");
			if(checkValidationToken(&str, head, token) == 1)
				return (1);
			i += 2;
		}
		else if(str[i] == '<' && str[i+1] == '<')
		{
			token = createToken(REDIR_HDOC, "<<");
			if(checkValidationToken(&str, head, token) == 1)
				return (1);
			i += 2;
		}
		else if(str[i] == '>')
		{
			token = createToken(REDIR_OUT, ">");
			if(checkValidationToken(&str, head, token) == 1)
				return (1);
			i++;
		}
		else if(str[i] == '<')
		{
			token = createToken(REDIR_IN, "<");
			if(checkValidationToken(&str, head, token) == 1)
				return (1);
			i++;
		}
		else if (str[i] == '|')
		{
			token = createToken(PIPE, "|");
			if(checkValidationToken(&str, head, token) == 1)
				return (1);
			i++;
		}
		else // attenzione: im not really checking the quotes here 
		{
			word = extractWord(str, &i);
			token = createToken(WORD, word);
			free(word);
			if(checkValidationToken(&str, head, token) == 1)
				return (1);
			printf("word: %s\n", word);
		}
		if(!head)
		{
			head = token;
			tail = token;
		}
		else
		{
			tail->next = token;
			tail = token;
		}
	}
	free(str);
}

/* [ ] change name enum (EOF)
   [ ] add quote filter in extract word
   [ ] mejorar tokenizeInput */