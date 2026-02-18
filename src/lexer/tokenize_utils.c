#include "minishell.h"

void	skip_space(char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
}

int	check_validation_token(char **str, t_token *head, t_token *token)
{
	if (!token)
	{
		free(*str);
		free_token_list(head);
		return (1);
	}
	return (0);
}

int	is_index_space_or_operator(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	else if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

char	*extract_word(char *str, int *i, t_minishell *data)
{
	int		start;
	int		in_d;
	int		in_s;
	char	*word;

	start = *i;
	in_d = 0;
	in_s = 0;
	data->unclosed_quotes = 0;
	while (str[*i])
	{
		if (str[*i] == '"' && !in_s)
			in_d = !in_d;
		else if (str[*i] == '\'' && !in_d)
			in_s = !in_s;
		else if (!in_d && !in_s && is_index_space_or_operator(str[*i]))
			break ;
		(*i)++;
	}
	word = malloc(sizeof(char) * (*i - start + 1));
	if (!word)
		return (NULL);
	ft_memcpy(word, &str[start], *i - start);
	return (word[*i - start] = '\0', word);
}

t_token	*check_operator(char *str, int *i)
{
	if (str[*i] == '>' && str[*i + 1] == '>')
		return (*i += 2, create_token(T_REDIR_APPEND, ">>"));
	if (str[*i] == '<' && str[*i + 1] == '<')
		return (*i += 2, create_token(T_REDIR_HDOC, "<<"));
	if (str[*i] == '>')
		return ((*i)++, create_token(T_REDIR_OUT, ">"));
	if (str[*i] == '<')
		return ((*i)++, create_token(T_REDIR_IN, "<"));
	if (str[*i] == '|')
		return ((*i)++, create_token(T_PIPE, "|"));
	return (NULL);
}
