/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:31:31 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:54:55 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const char	*temp_src;
	char		*temp_dst;
	size_t		i;

	i = 0;
	temp_dst = ((char *) dest);
	temp_src = ((char *) src);
	while (i < n)
	{
		temp_dst[i] = temp_src[i];
		i++;
	}
	return (dest);
}
