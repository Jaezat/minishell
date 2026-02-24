#include "minishell.h"

void	process_hdoc_line(char *line, int fd, int expand, t_minishell *data)
{
	char	*expanded;

	if (expand)
	{
		expanded = check_after_dollar_sign(line, data);
		free(line);
		line = expanded;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

char	*manage_hdoc(char *raw_delim, t_minishell *data)
{
	int		fd;
	int		expand;
	char	*line;
	char	*delim;

	expand = (ft_strchr(raw_delim, '\'') == NULL
			&& ft_strchr(raw_delim, '\"') == NULL);
	delim = remove_quotes(raw_delim);
	fd = open(".heredoc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(delim), NULL);
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		process_hdoc_line(line, fd, expand, data);
	}
	free(delim);
	close(fd);
	return (ft_strdup(".heredoc_temp"));
}

