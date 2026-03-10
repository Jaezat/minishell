/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 02:55:30 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/02 19:08:42 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	print_exit_error(char *error)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static int	is_bigger_than_limit(char *num, int is_negative)
{
	char	*lim;
	int		i;

	if (is_negative)
		lim = "9223372036854775808";
	else
		lim = "9223372036854775807";
	i = 0;
	while (i < 19)
	{
		if (num[i] > lim[i])
			return (1);
		if (num[i] < lim[i])
			return (0);
		i++;
	}
	return (0);
}

static int	is_more_than_long(char *str)
{
	int	i;
	int	is_negative;
	int	numbers;

	i = 0;
	is_negative = 0;
	if (str[i] == '+' || str[i] == '-')
		is_negative = (str[i++] == '-');
	while (str[i] == '0')
		i++;
	numbers = 0;
	while (str[i + numbers] >= '0' && str[i + numbers] <= '9')
		numbers++;
	if (numbers < 19)
		return (0);
	if (numbers > 19)
		return (1);
	return (is_bigger_than_limit(str + i, is_negative));
}

int	ft_exit(t_minishell *data, char **args)
{
	long	final_code;

	if (data->is_pipeline)
		return (0);
	ft_putstr_fd("exit\n", 2);
	if (!args[1])
	{
		final_code = data->exit_status;
		free_all_data(data);
		exit(final_code);
	}
	if (!ft_is_numeric(args[1]) || is_more_than_long(args[1]))
	{
		print_exit_error(args[1]);
		return (2);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	final_code = ft_atoll(args[1]);
	free_all_data(data);
	exit(final_code % 256);
}
