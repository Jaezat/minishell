/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:13:49 by mariacos          #+#    #+#             */
/*   Updated: 2025/11/01 19:54:13 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	number(char c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}

static int	is_space(char a)
{
	if (a == 32 || (a >= 9 && a <= 13))
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	nb_is_negative;

	i = 0;
	res = 0;
	nb_is_negative = 1;
	while (is_space(nptr[i]) == 1)
		i++;
	if (nptr[i] == '-')
	{
		nb_is_negative = 0;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (number(nptr[i]) == 1)
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	if (nb_is_negative == 0)
		res = -res;
	return (res);
}
