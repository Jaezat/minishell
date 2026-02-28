/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:47:25 by mariacos          #+#    #+#             */
/*   Updated: 2026/02/28 20:13:51 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	strlen_gnl(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	return (i);
}

char	*strjoin_n_free_gnl(char *s1, char *s2)
{
	char	*concat_str;
	size_t	s1_len;
	size_t	s2_copylen;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	s2_copylen = strlen_gnl(s2);
	s1_len = strlen_gnl(s1);
	concat_str = malloc(s1_len + s2_copylen + 1);
	if (!concat_str)
		return (free(s1), NULL);
	i = -1;
	while (++i < s1_len)
		concat_str[i] = s1[i];
	j = -1;
	while (++j < s2_copylen)
		concat_str[i + j] = s2[j];
	concat_str[i + j] = '\0';
	free(s1);
	return (concat_str);
}

/* char	*ft_strchr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	if (*str == '\0' && c == '\0')
		return (str);
	return (NULL);
} */
/* 
char	*ft_strdup(char *s1)
{
	char	*cpy_str;
	size_t	i;

	if (!s1)
		return (NULL);
	cpy_str = (char *)malloc(sizeof(*s1) * (strlen_gnl(s1) + 1));
	if (!cpy_str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		cpy_str[i] = s1[i];
		i++;
	}
	cpy_str[i] = '\0';
	return (cpy_str);
} */

void	clean_buffer(char *buffer)
{
	char	*nl_ptr;
	size_t	i;

	nl_ptr = ft_strchr(buffer, '\n');
	if (!nl_ptr)
	{
		buffer[0] = '\0';
		return ;
	}
	if (!*(nl_ptr + 1))
	{
		buffer[0] = '\0';
		return ;
	}
	i = 0;
	while ((nl_ptr)[i + 1])
	{
		buffer[i] = (nl_ptr)[i + 1];
		i++;
	}
	buffer[i] = '\0';
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*new_line;
	ssize_t		bytes_read;

	new_line = ft_strdup("");
	if (fd < 0 || BUFFER_SIZE <= 0 || !new_line)
		return (free(new_line), NULL);
	while (1 == 1)
	{
		if (!*buffer)
		{
			bytes_read = read(fd, buffer, BUFFER_SIZE);
			if (bytes_read < 0 || (bytes_read == 0 && !*new_line))
				return (free(new_line), NULL);
			if (bytes_read == 0)
				return (new_line);
			buffer[bytes_read] = '\0';
		}
		new_line = strjoin_n_free_gnl(new_line, buffer);
		if (!new_line)
			return (NULL);
		if (ft_strchr(new_line, '\n'))
			return (clean_buffer(buffer), new_line);
		buffer[0] = '\0';
	}
}