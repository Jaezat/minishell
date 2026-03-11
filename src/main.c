/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:25:40 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 09:27:15 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("Error: minishell does not accept arguments\n", 2);
		return (1);
	}
	handle_signals();
	data = init_all_data(envp);
	if (!data)
		return (1);
	start_operational_loop(data);
	free_all_data(data);
	return (0);
}
