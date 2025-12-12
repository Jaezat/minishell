#include "minishell.h"

void readline_calling(char **line)
{
    *line = readline(
        "\001\033[1;35m\002minishell$>\001\033[0m\002 "
    );
}

/* int is_interactive(t_data *data)
{
	if (data->interactive == 1)
    {
        readline_calling(&(data->line));
        if(data->line == NULL)
        {
            ft_printf("Exit");
            return 1;
        }
        if(data->line)
            add_history(data->line);
    }
    else
    {
        data->line = get_next_line(STDIN_FILENO);
        if(data->line == NULL)
            return (0);
    }
    if(*data->line == '\0')
    {
        return (1);
    }
	return (0);
} */

int is_interactive(t_data *data)
{
    readline_calling(&(data->line));
    if (data->line == NULL)
    {
        ft_printf("Exit");
        return 1;
    }
    if (data->line)
    {
        add_history(data->line);
    }
    return (0);
}

int count_envp(char **envp)
{
	int i;
	i = 0;
	while(envp[i])
		i++;
	return (i);
}

char **copy_envp(char **envp, int size_envp)
{
	int i;
	char **copy_env;

	i = 0;
	copy_env = malloc(sizeof(char *) * (size_envp + 1));
	if(!copy_env)
		return NULL;
	while(envp[i])
	{
		copy_env[i] = ft_strdup(envp[i]);
		i++;
	}
	copy_env[i] = NULL; 
	return copy_env;
}

t_data  *init_data(char **envp)
{
    t_data  *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    ft_memset(data, 0, sizeof(t_data)); 
	data->interactive = isatty(STDIN_FILENO); 
	data->size_envp = count_envp(envp);
	data->envp = copy_envp(envp, data->size_envp);
	if (!data->envp)
    {
        free(data);
        return (NULL);
    }  
    return (data);
}

/* int create_token(t_data)
{
    
    
} */

int operational_loop(t_data *data)
{
    while (1)
    {
        if (is_interactive(data) == 1)
            return 1;
    }
    printf("here");
    printf("%s\n", data->line); 
    return 0;
}



