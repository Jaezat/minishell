#include "minishell.h"

void	process_hdoc_line(char *line, int fd, int expand, t_minishell *data)
{
	char	*line_copy;
	char	*expanded;

	line_copy = ft_strdup(line);
	if (expand)
	{
		expanded = check_after_dollar_sign(line_copy, data);
		free(line_copy);
		line_copy = expanded;
	}
	write(fd, line_copy, ft_strlen(line_copy));
	write(fd, "\n", 1);
	free(line_copy);
}
