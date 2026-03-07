/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:28:45 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/07 13:35:45 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_unique_filename(t_minishell *shell)
{
	char	*num;
	char	*name;

	num = ft_itoa(shell->hdoc_counter);
	shell->hdoc_counter++;
	name = ft_strjoin("/tmp/.heredoc_", num);
	free(num);
	return (name);
}

int	delimiter_needs_expansion(char *raw_delimiter)
{
	return (ft_strchr(raw_delimiter, '\'') == NULL
		&& ft_strchr(raw_delimiter, '\"') == NULL);
}

int	is_pending_heredoc(char *delimiter)
{
	char	*nl_delimiter;

	nl_delimiter = ft_strchr(delimiter, '\n');
	if (!nl_delimiter)
		return (0);
	return (1);
}

int	is_closed_heredoc(char *nl_delimiter, char *real_delimiter)
{
	char	**lines;
	int		i;

	if (!nl_delimiter)
		return (0);
	lines = ft_split(nl_delimiter, '\n');
	i = 0;
	while (lines[i])
	{
		if (ft_strcmp(lines[i], real_delimiter) == 0)
		{
			free_2d_array(lines);
			return (1);
		}
		i++;
	}
	free_2d_array(lines);
	return (0);
}

int	get_bytes_to_write(char *nl_del, char *real_del)
{
	int	bytes_to_write;

	if (is_closed_heredoc(nl_del, real_del))
		bytes_to_write = ft_strlen(nl_del + 1) - ft_strlen(real_del);
	else
		bytes_to_write = ft_strlen(nl_del + 1);
	return (bytes_to_write);
}
