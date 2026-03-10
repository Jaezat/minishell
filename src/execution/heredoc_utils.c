/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:28:39 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/08 14:39:35 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pending_heredoc(char *delimiter, int fd)
{
	char	*real_delimiter;
	char	*nl_delimiter;
	char	**lines;
	int		bytes_to_write;
	int		i;

	nl_delimiter = ft_strchr(delimiter, '\n');
	real_delimiter = ft_substr(delimiter, 0, (nl_delimiter - delimiter));
	bytes_to_write = get_bytes_to_write(nl_delimiter, real_delimiter);
	write(fd, nl_delimiter + 1, bytes_to_write);
	lines = ft_split(nl_delimiter, '\n');
	i = 0;
	while (lines[i])
	{
		if (ft_strcmp(lines[i], real_delimiter) == 0)
		{
			free_2d_array(lines);
			return (free(real_delimiter), 1);
		}
		i++;
	}
	write(fd, "\n", 1);
	free_2d_array(lines);
	return (free(real_delimiter), 0);
}

void	handle_heredoc_sigint(int signal)
{
	g_signal_status = signal;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	handle_heredoc_signals(int *stdin_backup, char **line)
{
	dup2(*stdin_backup, STDIN_FILENO);
	close(*stdin_backup);
	handle_signals();
	free(*line);
}

int	handle_regular_heredoc(char *del, int exp, int fd, t_minishell *shell)
{
	char	*line;
	int		stdin_backup;

	signal(SIGINT, handle_heredoc_sigint);
	stdin_backup = dup(STDIN_FILENO);
	while (1)
	{
		line = readline("> ");
		if (g_signal_status == SIGINT)
			return (handle_heredoc_signals(&stdin_backup, &line), 1);
		if (!line)
		{
			heredoc_endoffile_error(del);
			break ;
		}
		if (ft_strcmp(line, del) == 0)
		{
			free(line);
			break ;
		}
		process_hdoc_line(line, fd, exp, shell);
		free(line);
	}
	close(stdin_backup);
	return (handle_signals(), 0);
}

char	*get_real_delimiter(char *delimiter)
{
	char	*nl_delimiter;
	char	*real_delimiter;

	nl_delimiter = ft_strchr(delimiter, '\n');
	real_delimiter = ft_substr(delimiter, 0, (nl_delimiter - delimiter));
	return (real_delimiter);
}
