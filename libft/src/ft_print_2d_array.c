/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_2d_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 10:00:56 by mariacos          #+#    #+#             */
/*   Updated: 2026/01/24 10:05:32 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_2d_array(char **array)
{
	int	i;
	int	j;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[i][j])
		{
			write(1, &array[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}
