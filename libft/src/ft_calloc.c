/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:41:01 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:54:30 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*mem;
	size_t	result;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	result = nmemb * size;
	if (nmemb != (result / size))
		return (NULL);
	mem = malloc(nmemb * size);
	if (mem == NULL)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
	{
		mem[i] = 0;
		i++;
	}
	return (mem);
}
