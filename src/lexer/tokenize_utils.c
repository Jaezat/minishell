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
	else if (ft_strchr("><;)}", c))
		return (1);
	return (0);
}

static int	handle_quote_char(char *str, int *i, char *quote)
{
	if ((str[*i] == '"' || str[*i] == '\'') && !*quote)
		*quote = str[(*i)++];
	else if (*quote && str[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
	}
	else
		return (0);
	return (1);
}

char	*extract_word(char *str, int *i, t_minishell *data)
{
	int		start;
	char	quote;

	start = *i;
	quote = 0;
	(void)data;
	while (str[*i])
	{
		if (!handle_quote_char(str, i, &quote))
		{
			if (!quote && is_index_space_or_operator(str[*i]))
				break ;
			(*i)++;
		}
	}
	if (quote != 0)
		return (print_error_unclosed_quote(quote), NULL);
	return (ft_substr(str, start, *i - start));
}
