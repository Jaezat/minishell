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

static int	write_heredoc_to_file(int fd, char *raw_delimiter, t_minishell *shell)
{
	char	*line;
	int		expand;
	char	*delimiter;
	char	*nl_delimiter;
	char	*tmp_delimiter;
	char	**lines_in_fd;
	int		i;

	expand = (ft_strchr(raw_delimiter, '\'') == NULL
			&& ft_strchr(raw_delimiter, '\"') == NULL);
	delimiter = remove_quotes(raw_delimiter);
	//if delimiter has new_line
	// delimiter becames what is before the new line
	// call process_hdoc_line() take everything after new
	nl_delimiter = ft_strchr(delimiter, '\n');
	if (nl_delimiter)
	{	
		tmp_delimiter = ft_substr(delimiter, 0, (nl_delimiter - delimiter));
		delimiter = tmp_delimiter;
		write(fd, nl_delimiter + 1, (ft_strlen(nl_delimiter + 1) - ft_strlen(delimiter)));
		/* write(fd, "\n", 1); */
		lines_in_fd = ft_split(nl_delimiter, '\n');
		i = 0;
		while (lines_in_fd[i])
		{
			if (ft_strcmp(lines_in_fd[i], delimiter) == 0)
				return (0);
			i++;
		}
	
	}
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
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		process_hdoc_line(line, fd, expand, shell);
		free(line);
	}
	free(delimiter);
	return (0);
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
