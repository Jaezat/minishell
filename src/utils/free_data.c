#include "minishell.h"

void	free_all_data(t_minishell *data)
{
	if (!data)
		return ;
	if (data->cmds)
	{
		free_cmd_list(data->cmds);
		data->cmds = NULL;
	}
	if (data->list_tokens)
	{
		free_token_list(data->list_tokens);
		data->list_tokens = NULL;
	}
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->env_list)
		free_env_list(data->env_list);
	free(data);
}

void	free_redir_list(t_redir *head)
{
	t_redir	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->file);
		free(head);
		head = tmp;
	}
}

void	free_cmd_list(t_cmd *head)
{
	t_cmd	*tmp;
	int		i;

	while (head)
	{
		tmp = head->next;
		if (head->args)
		{
			i = 0;
			while (head->args[i])
				free(head->args[i++]);
			free(head->args);
		}
		free_redir_list(head->redirs);
		free(head);
		head = tmp;
	}
}

void	free_struct_assistant(t_struct *st, int fail)
{
	if (!st)
		return ;
	if (fail && st->head_cmd)
		free_cmd_list(st->head_cmd);
	free(st);
}
