/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:50:38 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:55:38 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	copy_strings(char *dst, const char *s1, const char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		dst[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		dst[i] = s2[j];
		i++;
		j++;
	}
	dst[i] = '\0';
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*both_strings;
	int		total_len;

	if (!s1 || !s2)
		return (NULL);
	total_len = (ft_strlen(s1) + ft_strlen(s2)) + 1;
	both_strings = (char *)malloc(sizeof(char) * total_len);
	if (!both_strings)
		return (NULL);
	copy_strings(both_strings, s1, s2);
	return (both_strings);
}
