#include "minishell.h"


void readline_calling(char **line)
{
	if (isatty(STDIN_FILENO))
		*line = readline("\001\033[1;35m\002minishell$>\001\033[0m\002 ");
	else
		*line = readline(NULL);
}

int has_unclosed_quotes(char *str)
{
	int in_single;
	int in_double;
	
	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (*str == '"' && !in_single)
			in_double = !in_double;
		else if (*str == '\'' && !in_double)
			in_single = !in_single;
		str++;
	}
	return (in_single || in_double);
}

char *read_complete_line(void)
{
    char    *line;

    if (!isatty(STDIN_FILENO))
    {
        line = get_next_line(STDIN_FILENO);
        if (line)
        {
            int len = ft_strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';
        }
        return (line);
    }
    line = readline("\001\033[1;35m\002minishell$>\001\033[0m\002 ");
	if (!line)
        return (NULL);
    return (line);
}

int is_interactive(t_minishell *data)
{
	if (data->line)
		free(data->line);
	data->line = read_complete_line();
	if (!data->line)
		return (1);
	if (*data->line)
		add_history(data->line);
	return (0);
}


int start_operational_loop(t_minishell *data)
{
    t_cmd   *cmds;

	handle_signals();
	cmds = NULL;
    while (1)
    {
        if (is_interactive(data) == 1) 
		{
			ft_putstr_fd("exit\n", 2);
			free_all_data(data);
			exit(0);
		}
        if (tokenize_input(data) == 0)
        {
            if (check_syntax(data->list_tokens) != 0)
            {
				data->exit_status = 2;
                free_token_list(data->list_tokens);
				data->list_tokens = NULL;
                continue;
            }
			// print_tokens(data);
            cmds = create_struct(data);
			free_token_list(data->list_tokens); 
    		data->list_tokens = NULL;
            if (cmds == NULL)
                continue;
			data->cmds = cmds;
			// print_commands(cmds);
			execute_commands(data, cmds);
			free_cmd_list(cmds);
			cmds = NULL;
			data->cmds = NULL;
    	}
		else
			data->exit_status = 2;
		if (cmds)
        	free_cmd_list(cmds);
	}
    return (0);
}
