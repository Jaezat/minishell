/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:25:36 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 09:30:10 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error_unclosed_quote(char quote)
{
	ft_putstr_fd("minishell: syntax error: missing matching ", 2);
	ft_putstr_fd("'", 2);
	ft_putchar_fd(quote, 2);
	ft_putstr_fd("'", 2);
	ft_putchar_fd('\n', 2);
	return (1);
}

int	print_error_syntax(char *str)
{
	char	*err_msg;

	err_msg = "minishell: syntax error near unexpected token ";
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

void	print_env_list(t_env *head)
{
	t_env	*current;
	int		i;

	current = head;
	i = 0;
	while (current)
	{
		printf("[%d] %s=%s\n", i, current->key, current->value);
		current = current->next;
		i++;
	}
	printf("Total: %d variables\n", i);
}

void	print_tokens(t_minishell *data)
{
	t_token	*current;

	current = data->list_tokens;
	while (current)
	{
		if (current->type == T_WORD)
			printf("WORD: %s\n", current->value);
		else if (current->type == T_PIPE)
			printf("PIPE: %s\n", current->value);
		else if (current->type == T_REDIR_IN)
			printf("REDIR_IN: %s\n", current->value);
		else if (current->type == T_REDIR_OUT)
			printf("REDIR_OUT: %s\n", current->value);
		else if (current->type == T_REDIR_HDOC)
			printf("REDIR_HDOC: %s\n", current->value);
		else if (current->type == T_REDIR_APPEND)
			printf("REDIR_APPEND: %s\n", current->value);
		else
			printf("EOF: %s\n", current->value);
		current = current->next;
	}
}
