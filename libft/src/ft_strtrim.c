/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:58:25 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:56:05 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

static int	calculate_space(int indexfront, int indexback)
{
	return (indexback - indexfront + 1);
}

static int	findchar(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	while (s1[i] != '\0')
	{
		j = 0;
		found = 0;
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (found == 0)
			return (i);
		i++;
	}
	return (0);
}

static int	findchar_reverse(char const *s1, char const *set)
{
	int	i;
	int	found;
	int	j;

	i = ft_strlen(s1) - 1;
	while (i >= 0)
	{
		j = 0;
		found = 0;
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (found == 0)
			return (i);
		i--;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		indexfront;
	int		indexback;
	int		space;
	int		j;
	char	*s3;

	if (!s1 || !set)
		return (NULL);
	indexfront = findchar(s1, set);
	indexback = findchar_reverse(s1, set);
	if (indexfront > indexback)
		return (ft_strdup(""));
	space = calculate_space(indexfront, indexback);
	s3 = (char *)malloc(space + 1);
	if (!s3)
		return (NULL);
	j = 0;
	while (indexfront <= indexback)
	{
		s3[j] = s1[indexfront];
		j++;
		indexfront++;
	}
	s3[j] = '\0';
	return (s3);
}
