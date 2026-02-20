#include "minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
	{
		token->value = ft_strdup(value);
		if (!token->value)
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

void	add_token(t_token **head, t_token **tail, t_token *token)
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

static t_token	*word_to_token(t_minishell *data, int *i)
{
	t_token	*token;
	char	*word;

	word = extract_word(data->line, i, data);
	if (!word)
		return (NULL);
	token = create_token(T_WORD, word);
	free(word);
	return (token);
}

/* int	tokenize_input(t_minishell *data)
{
	int		i;
	t_token	*token;
	t_token	*head;
	t_token	*tail;

	i = 0;
	head = NULL;
	tail = NULL;
	while (data->line[i])
	{
		skip_space(data->line, &i);
		if (!data->line[i])
			break ;
		token = check_operator(data->line, &i);
		if (!token)
			token = word_to_token(data, &i);
		if (!token || check_validation_token(&data->line, head, token))
			return (free_token_list(head), 1);
		add_token(&head, &tail, token);
	}
	return (data->list_tokens = head, 0);
} */

int	tokenize_input(t_minishell *data)
{
	int		i;
	t_token	*token;
	t_token	*head;
	t_token	*tail;

	i = 0;
	head = NULL;
	tail = NULL;
	while (data->line[i])
	{
		skip_space(data->line, &i);
		if (!data->line[i])
			break ;
		token = check_operator(data->line, &i); 
		if (!token)
		{
			token = word_to_token(data, &i); 
		}
		if (!token || check_validation_token(&data->line, head, token))
			return (free_token_list(head), 1);
		add_token(&head, &tail, token); 
	}
	return (data->list_tokens = head, 0);
}
