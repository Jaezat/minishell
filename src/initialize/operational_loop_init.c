/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operational_loop_init.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:24:23 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 10:55:00 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_and_execute(t_minishell *data, t_cmd **cmds)
{
	if (check_syntax(data->list_tokens) != 0)
	{
		data->exit_status = 2;
		free_token_list(data->list_tokens);
		data->list_tokens = NULL;
		return (1);
	}
	*cmds = create_struct(data);
	free_token_list(data->list_tokens);
	data->list_tokens = NULL;
	if (!*cmds)
		return (1);
	data->cmds = *cmds;
	execute_commands(data, *cmds);
	free_cmd_list(*cmds);
	*cmds = NULL;
	data->cmds = NULL;
	return (0);
}

int	start_operational_loop(t_minishell *data)
{
	t_cmd	*cmds;

	cmds = NULL;
	while (1)
	{
		data->is_pipeline = 0;
		if (is_interactive(data) == 1)
		{
			ft_putstr_fd("exit\n", 2);
			free_all_data(data);
			exit(0);
		}
		if (g_signal_status == SIGINT)
			data->exit_status = 130;
		g_signal_status = 0;
		if (tokenize_input(data) == 0)
			parse_and_execute(data, &cmds);
		if (cmds)
			free_cmd_list(cmds);
	}
	return (0);
}
