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
	{
		data->exit_status = 2;
		return (NULL);
	}
	token = create_token(T_WORD, word);
	free(word);
	return (token);
}

<<<<<<< Updated upstream
static int	fill_token_list(t_minishell *data,
	t_token **head, t_token **tail)
=======
static int	fill_token_list(t_minishell *data, t_token **head, t_token **tail)
>>>>>>> Stashed changes
{
	int		i;
	t_token	*token;

	i = 0;
	while (data->line[i])
	{
		skip_space(data->line, &i);
		if (!data->line[i])
			break ;
		token = check_operator(data->line, &i);
		if (!token)
			token = word_to_token(data, &i);
		if (!token || check_validation_token(&data->line, *head, token))
			return (free_token_list(*head), 1);
		add_token(head, tail, token);
	}
	return (0);
}

int	tokenize_input(t_minishell *data)
{
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	if (ft_strcmp(data->line, "\"\"") == 0)
	{
		ft_putstr_fd("minishell: Command '' not found.\n", 2);
		data->exit_status = 127;
		return (1);
	}
	if (fill_token_list(data, &head, &tail))
		return (1);
	return (data->list_tokens = head, 0);
}
