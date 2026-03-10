/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 13:28:12 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/10 15:58:29 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_path_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	print_absolute_path_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

int	check_fd_error(int fd, char *filename)
{
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(filename);
		return (-1);
	}
	return (0);
}

void	heredoc_endoffile_error(char *del)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file ", 2);
	ft_putstr_fd("(wanted '", 2);
	ft_putstr_fd(del, 2);
	ft_putstr_fd("')\n", 2);
}
