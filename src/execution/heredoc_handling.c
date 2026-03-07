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

static int	write_heredoc_to_file(int fd, char *raw_delimiter, t_minishell *shell)
{
	int		expand;
	int		status;
	char	*delimiter;
	char	*real_delimiter;

	status = 0;
	delimiter = remove_quotes(raw_delimiter);
	expand = delimiter_needs_expansion(raw_delimiter);
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
	status = handle_regular_heredoc(delimiter, expand, fd, shell);
	free(delimiter);
	return (status);
}

static int	process_heredoc(t_redir *redir, t_minishell *shell)
{
	int		fd;
	char	*filename;
	int		status;

	filename = generate_unique_filename(shell);
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
