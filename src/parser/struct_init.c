#include "minishell.h"

t_cmd	*create_node(void)
{
	t_cmd	*new_cmd;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	return (new_cmd);
}

t_struct	*init_struct(t_minishell *data)
{
	t_struct	*st;

	st = malloc(sizeof(t_struct));
	if (!st)
		return (NULL);
	st->head_cmd = create_node();
	if (!st->head_cmd)
	{
		free(st);
		return (NULL);
	}
	st->current_cmd = st->head_cmd;
	st->current_tkn = data->list_tokens;
	st->arg_index = 0;
	st->current_cmd->size_args = count_size_args(st->current_tkn);
	st->current_cmd->args = ft_calloc
		(st->current_cmd->size_args + 1, sizeof(char *));
	if (!st->current_cmd->args)
	{
		free_cmd_list(st->head_cmd);
		free(st);
		return (NULL);
	}
	return (st);
}

void	add_redir_node(t_cmd *cmd, t_token_type type, char *expanded_file)
{
	t_redir	*current_redir;
	t_redir	*last;

	current_redir = malloc(sizeof(t_redir));
	if (!current_redir)
		return ;
	current_redir->type = type;
	current_redir->file = ft_strdup(expanded_file);
	current_redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = current_redir;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = current_redir;
	}
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	in_quote;
	char	*str_clean;

	i = 0;
	j = 0;
	in_quote = 0;
	if (!str)
		return (NULL);
	str_clean = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!str_clean)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && in_quote == 0)
			in_quote = str[i];
		else if (str[i] == in_quote)
			in_quote = 0;
		else
			str_clean[j++] = str[i];
		i++;
	}
	str_clean[j] = '\0';
	return (str_clean);
}
