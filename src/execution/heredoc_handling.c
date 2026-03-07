/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:28:26 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/28 20:56:38 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*generate_unique_filename(void)
{
	static int i = 0;
	char *num;
	char *name;

	num = ft_itoa(i++);
	name = ft_strjoin("/tmp/.heredoc_", num);
	free(num);
	return (name);
}

static int	delimiter_needs_expansion(char *raw_delimiter)
{
	return (ft_strchr(raw_delimiter, '\'') == NULL
			&& ft_strchr(raw_delimiter, '\"') == NULL);
}

static int	is_pending_heredoc(char *delimiter)
{
	char	*nl_delimiter;

	nl_delimiter = ft_strchr(delimiter, '\n');
	if (!nl_delimiter)
		return (0);
	return (1);
}

static int	is_closed_heredoc(char *nl_delimiter, char *real_delimiter)
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
			return (1);
		i++;
	}
	free_2d_array(lines);
	return (0);
}

static int	get_bytes_to_write(char *nl_delimiter, char *real_delimiter)
{
	int	bytes_to_write;

	if (is_closed_heredoc(nl_delimiter, real_delimiter))
		bytes_to_write = ft_strlen(nl_delimiter + 1) - ft_strlen(real_delimiter);
	else
		bytes_to_write = ft_strlen(nl_delimiter + 1);
	return (bytes_to_write);
}

static int	handle_pending_heredoc(char *delimiter, int fd)
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
			return (1);
		i++;
	}
	write(fd, "\n", 1);
	free_2d_array(lines);
	return (0);
}

static int	handle_regular_heredoc(char *del, int expand, int fd, t_minishell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
			break;
		}
		if (g_signal_status == SIGINT)
		{
			free(line);
			return (1);
		}
		if (ft_strcmp(line, del) == 0)
		{
			free(line);
			break;
		}
		process_hdoc_line(line, fd, expand, shell);
		free(line);
	}
	return (0);
}

static char	*get_real_delimiter(char *delimiter)
{
	char	*nl_delimiter;
	char	*real_delimiter;

	nl_delimiter = ft_strchr(delimiter, '\n');
	real_delimiter = ft_substr(delimiter, 0, (nl_delimiter - delimiter));
	return (real_delimiter);
}

static int	write_heredoc_to_file(int fd, char *raw_delimiter, t_minishell *shell)
{
	int		expand;
	int		status;
	char	*delimiter;
	char	*real_delimiter;

	status = 0;
	delimiter = remove_quotes(raw_delimiter);
	if (is_pending_heredoc(delimiter))
	{
		if (handle_pending_heredoc(delimiter, fd))
			return (free(delimiter), 0);
		real_delimiter = get_real_delimiter(delimiter);
		if (!real_delimiter)
			return (free(delimiter), 0);
		free(delimiter);
		delimiter = real_delimiter;
	}
	expand = delimiter_needs_expansion(raw_delimiter);
	status = handle_regular_heredoc(delimiter, expand, fd, shell);
	free(delimiter);
	return (status);
}

static int	process_heredoc(t_redir *redir, t_minishell *shell)
{
	int		fd;
	char	*filename;
	int		status;

	filename = generate_unique_filename();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc");
		free(filename);
		return (1);
	}
	status = write_heredoc_to_file(fd, redir->file, shell);
	close(fd);
	if (status == 1)
	{
		unlink(filename);
		free(filename);
		return (1);
	}
	free(redir->file);
	redir->file = filename;
	redir->type = T_REDIR_IN;
	return (0);
}

int	handle_heredocs(t_cmd *cmd, t_minishell *shell)
{
	t_redir *redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == T_REDIR_HDOC)
			{
				if (process_heredoc(redir, shell) == 1)
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
