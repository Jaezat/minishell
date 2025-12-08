/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 14:25:21 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/07 18:05:51 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* [ ] Argument verification
    -Check parameters: give warning if (argc != 1)
    -Should we also already here deal with seeing if
    im able to run the program from a script or in a non interactive
    way
    Ej:
    echo "ls" > test.txt
    bash < test.txt // i should have the same behaviour
*/

/************************/

/* 1. Check parameters: give warning if (argc != 1) */

void free_all(s_data *data)
{
	if(!data)
		return ;
	if(data->line)
		free(data->line);
	if(data->envp)
		free_2d_array(data->envp);
	free(data);
		
}

int is_interactive(s_data *data)
{
	if (data->interactive == 1)
    {
        /* si es que es el terminal tengo que usar readline() para
        acceder a lo que el terminal escribio.
        Readline() will read a line from tty and return it(must free when done with it)
        if line is empty = return NULL */
        data->line = readline("minishell$> ");
        if(data->line == NULL)
        {
            ft_printf("Exit");
            return 1;
        }
        if(data->line)
        {
            add_history(data->line);
        }
    }
    else
    {
        /* if the prompt doesnt come from the tty if normally comes from a file
        for ex, for that we would need to access to whatever is written in the
        file so we can use GNL to get what is written there */
        data->line = get_next_line(STDIN_FILENO);
        /*lo que pasa aca es que el shell directamente redirecciona
        el stdin del terminal al stdin del file que estamos usando*/    
    }
    if(*data->line == '\0')
    {
        return (1); // que hacemos en caso que la linea este vacia?
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

s_data  *init_data(char **envp)
{
    s_data  *data;

    data = malloc(sizeof(s_data));
    if (!data)
        return (NULL);
    ft_memset(data, 0, sizeof(s_data)); 
	data->interactive = isatty(STDIN_FILENO);
	data->size_envp = count_envp(envp);
	data->envp = copy_envp(envp, data->size_envp);
	if (!data->envp)
    {
        free(data);
        return (NULL);
    }  
	//signal frunction calling here maybe? Not sure
    return (data);
}


int main(int argc, char **argv, char **envp)
{
    s_data  *data;
    
    (void)argv;
    if (argc != 1)
    {
        ft_putstr_fd("Error: minishell does not accept arguments\n", 2);
        return (1);
    }
    data = init_data(envp);
    if (!data)
        return (1);
    while (1)
    {
        if (is_interactive(data) == 1)
        {
            free_all(data);
            return 1;
        }
		char **tokens = ft_split(data->line, ' ');
		int i = 0;
		while(tokens[i])
		{
			printf("Token %d: %s\n", i, tokens[i]);
			i++;
		}
    }
    free_all(data);
    return (0);
}


