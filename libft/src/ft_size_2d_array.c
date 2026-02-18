/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_size_2d_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 19:49:58 by mariacos          #+#    #+#             */
/*   Updated: 2026/02/17 19:50:08 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_size_2d_array(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return (0);
	while (matrix[i])
		i++;
	return (i);
}
