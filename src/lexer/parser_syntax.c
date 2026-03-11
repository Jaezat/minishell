/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:24:51 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 09:24:52 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_operator(char *str, int *i)
{
	char	s[2];

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
	if (ft_strchr(";&()}{", str[*i]))
	{
		s[0] = str[*i];
		s[1] = '\0';
		(*i)++;
		return (create_token(T_ERROR, s));
	}
	return (NULL);
}

int	check_redirec(t_token *token)
{
	if (token->type != T_REDIR_IN && token->type != T_REDIR_OUT
		&& token->type != T_REDIR_APPEND && token->type != T_REDIR_HDOC)
		return (0);
	if (token->next == NULL)
		return (print_error_syntax("newline"));
	if (token->next->type != T_WORD)
		return (print_error_syntax(token->next->value));
	return (0);
}

int	check_syntax(t_token *list_tokens)
{
	t_token	*current;
	int		status;

	current = list_tokens;
	while (current != NULL)
	{
		if (current->type == T_ERROR)
			return (print_error_syntax(current->value));
		if (current->type == T_PIPE)
		{
			if (current == list_tokens)
				return (print_error_syntax("|"));
			if (current->next == NULL)
				return (print_error_syntax("newline"));
			if (current->next->type < T_WORD || current->next->type
				> T_REDIR_HDOC)
				return (print_error_syntax(current->next->value));
		}
		status = check_redirec(current);
		if (status != 0)
			return (status);
		current = current->next;
	}
	return (0);
}
