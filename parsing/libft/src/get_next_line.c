/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:47:25 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/02 12:19:01 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *gnl_strjoin(char *s1, char *s2, size_t len)
{
    char    *new;
    size_t  i;
    size_t  j;
    size_t  s1_len;

    if (s1)
        s1_len = ft_strlen(s1);
    else
        s1_len = 0;
    new = (char *)malloc(s1_len + len + 1);
    if (!new)
        return (NULL);
    i = 0;
    while (s1 && s1[i])
    {
        new[i] = s1[i];
        i++;
    }
    j = -1;
    while (++j < len)
        new[i + j] = s2[j];
    new[i + j] = '\0';
    if (s1)
        free(s1);
    return (new);
}

static char	*ext(char *buffer, int *offset, size_t bytes_read)
{
	size_t		i;
	size_t		length_part;
	char	*part;

	i = *offset;
	while (i < bytes_read && buffer[i] != '\0')
	{
		if (buffer[i] == '\n')
		{
			i++;
			break ;
		}
		i++;
	}
	length_part = i - *offset;
	part = ft_substr(buffer, *offset, length_part);
	*offset = i;
	return (part);
}

static char	*append_next(char *line, char *buffer, int *offset, size_t bytes_read)
{
	char	*part;
	char	*new_line;

	part = ext(buffer, offset, bytes_read);
	if (!part)
	{
		free(line);
		return (NULL);
	}
	new_line = gnl_strjoin(line, part, ft_strlen(part));
	free(part);
	if (!new_line)
	{
		free(line);
		return (NULL);
	}
	return (new_line);
}

static int	check(int fd, char *buffer, size_t *bytes_read, char **line)
{
	*bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		if (*line)
		{
			free(*line);
			*line = NULL;
		}
		return (-1);
	}
	if (*bytes_read == 0)
		return (0);
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	static int	offset;
	static size_t	bytes_read;
	char		*line;
	int			line_complete;

	line = NULL;
	line_complete = 0;
	while (!line_complete)
	{
		if ((offset >= (int)bytes_read) && (bytes_read <= INT_MAX))
		{
			if (check(fd, buffer, &bytes_read, &line) <= 0)
				return (line);
			offset = 0;
		}
		line = append_next(line, buffer, &offset, bytes_read);
		if (!line)
			return (NULL);
		if (buffer[offset - 1] == '\n')
			line_complete = 1;
	}
	return (line);
}


