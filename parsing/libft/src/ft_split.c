/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:52:39 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/04 15:00:43 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	numofwords(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static int	allocmemwords(char **array, char *s, char c)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		len = 0;
		while (s[i + len] && s[i + len] != c)
			len++;
		if (len > 0)
		{
			array[j] = (char *)malloc(sizeof(char) * (len + 1));
			if (!array[j])
				return (1);
			j++;
			i += len;
		}
	}
	return (0);
}

static int	addingvaluesarrays(char **array, char *s, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		k = 0;
		while (s[i] && s[i] != c)
		{
			array[j][k] = s[i];
			i++;
			k++;
		}
		if (k > 0)
		{
			array[j][k] = '\0';
			j++;
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**array_s;
	char	*s_casted;
	int		total;

	s_casted = (char *)s;
	if (!s_casted)
		return (NULL);
	total = numofwords(s_casted, c);
	array_s = (char **)malloc(sizeof(char *) * (total + 1));
	if (!array_s)
		return (NULL);
	array_s[total] = (NULL);
	if (allocmemwords(array_s, s_casted, c))
	{
		free_2d_array(array_s);
		return (NULL);
	}
	addingvaluesarrays(array_s, s_casted, c);
	return (array_s);
}
/* #include <stdio.h>

int main(int ac, char *av[])
{
    char    **ptr = ft_split(av[1], av[2][0]);

    if (ac == 3)
    {
        while (*ptr != NULL)
        {
            printf("%s\n", *ptr);
            ptr++;
        }
    }
    return (0);
} */