/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:28:26 by andcardo          #+#    #+#             */
/*   Updated: 2026/02/25 16:46:30 by andcardo         ###   ########.fr       */
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

static int	write_heredoc_to_file(int fd, char *limiter)
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
		if (g_signal_status == SIGINT) // Ctrl-C
		{
			free(line);
			return (1); // Error code
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd); // Write to file
		free(line);
	}
	return (0); // Success
}

static int	process_heredoc(t_redir *redir)
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
	status = write_heredoc_to_file(fd, redir->file);
	close(fd);
	if (status == 1) // Signal interruption
	{
		unlink(filename); // Delete the file immediately
		free(filename);
		return (1);
	}
	// Success: Transform the node
	free(redir->file);
	redir->file = filename;
	redir->type = T_REDIR_IN;
	return (0);
}

int	handle_heredocs(t_cmd *cmd)
{
	t_redir *redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == T_REDIR_HDOC)
			{
				if (process_heredoc(redir) == 1)
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
