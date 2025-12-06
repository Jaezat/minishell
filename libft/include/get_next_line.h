/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:12:52 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/02 11:33:43 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>




/* char		*get_next_line(int fd);
char	    *append_next(char *line, char *buffer, int *offset, size_t bytes_read);
char		*ext(char *buffer, int *offset, size_t bytes_read);
char	    *gnl_strjoin(char *s1, char *s2, size_t len); */

char	*get_next_line(int fd);

#endif
