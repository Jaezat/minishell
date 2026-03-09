#include "minishell.h"

void	readline_calling(char **line)
{
	if (isatty(STDIN_FILENO))
		*line = readline("\001\033[1;35m\002minishell$>\001\033[0m\002 ");
	else
		*line = readline(NULL);
}

char	*read_complete_line(void)
{
	char	*line;
	int		len;

	if (!isatty(STDIN_FILENO))
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
		return (line);
	}
	line = readline("\001\033[1;35m\002minishell$>\001\033[0m\002 ");
	if (!line)
		return (NULL);
	return (line);
}

int	is_interactive(t_minishell *data)
{
	if (data->line)
		free(data->line);
	data->line = read_complete_line();
	if (!data->line)
		return (1);
	if (*data->line)
		add_history(data->line);
	return (0);
}

int	has_unclosed_quotes(char *str)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (*str == '"' && !in_single)
			in_double = !in_double;
		else if (*str == '\'' && !in_double)
			in_single = !in_single;
		str++;
	}
	return (in_single || in_double);
}
