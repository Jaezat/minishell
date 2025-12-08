/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:38:50 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:54:47 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int	count_size(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*handle_int_min(void)
{
	char	*min_str;
	char	*literal;
	int		i;

	i = 0;
	min_str = (char *)malloc(sizeof(char) * 12);
	if (!min_str)
		return (NULL);
	literal = "-2147483648";
	while (i < 12)
	{
		min_str[i] = literal[i];
		i++;
	}
	return (min_str);
}

void	fill_array(char *array, int n, int i, int is_negative)
{
	int	rest;

	if (n == 0)
		array[0] = '0';
	if (is_negative)
	{
		array[0] = '-';
		n = -n;
	}
	while (n > 0 && i >= is_negative)
	{
		rest = n % 10;
		array[i] = rest + '0';
		n = n / 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	int		size;
	int		is_negative;
	int		i;
	char	*array;

	if (n == -2147483648)
		return (handle_int_min());
	size = count_size(n);
	array = (char *) malloc(sizeof(char) * (size + 1));
	if (!array)
		return (NULL);
	array[size] = '\0';
	is_negative = 0;
	if (n < 0)
		is_negative = 1;
	i = size - 1;
	fill_array(array, n, i, is_negative);
	return (array);
}
