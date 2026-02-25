#include "minishell.h"

void	print_path_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void check_fd_error(int fd, char *filename)
{
	if (fd == -1)
	{
		perror(filename);
		exit(1);
	}
}

