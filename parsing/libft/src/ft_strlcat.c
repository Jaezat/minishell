/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:07:53 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:55:41 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;
	size_t	j;
	size_t	bytes_to_copy;

	i = 0;
	j = 0;
	dest_len = ft_strlen(dst);
	src_len = 0;
	while (src[src_len] != '\0')
		src_len++;
	bytes_to_copy = size - dest_len - 1;
	if (size <= dest_len)
		return (size + src_len);
	while (j < bytes_to_copy && src[i] != '\0')
	{
		dst[dest_len + i] = src[i];
		i++;
		j++;
	}
	dst[dest_len + i] = '\0';
	return (dest_len + src_len);
}
