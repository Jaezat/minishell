/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 19:03:07 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:54:50 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*temp_s;
	unsigned char		temp_c;
	size_t				i;

	temp_s = (const unsigned char *)s;
	temp_c = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (temp_s[i] == temp_c)
		{
			return ((void *)(temp_s + i));
		}
		i++;
	}
	return (NULL);
}
