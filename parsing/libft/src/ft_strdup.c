/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 21:39:32 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:55:26 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	char	*copy_s1;
	int		size_s;
	int		i;

	i = 0;
	size_s = ft_strlen(s1);
	copy_s1 = malloc(sizeof(char) * (size_s + 1));
	if (copy_s1 == NULL)
		return (NULL);
	while (s1[i])
	{
		copy_s1[i] = s1[i];
		i++;
	}
	copy_s1[i] = '\0';
	return (copy_s1);
}
