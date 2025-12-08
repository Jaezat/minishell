/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 14:25:21 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/05 16:39:08 by mariacos         ###   ########.fr       */
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

int main(int argc, char **argv, char **envp)
{
    if (argc != 1)
    {
        printf("Error: minishell does not accept arguments\n");
        return (1);
    }
    /* Check is being run from iterative mode or non interative mode */
    
    s_init_var *init = malloc(sizeof(s_init_var));
    if(!init)
        return NULL;

    init->interactive = isatty(STDIN_FILENO);
    if (init->interactive == 1)
    {
        /* si es que es el terminal tengo que usar readline() para
        acceder a lo que el terminal escribio.
        Readline() will read a line from tty and return it(must free when done with it)
        if line is empty = return NULL */
        init->line = readline("minishell$ ");
        if(init->line == NULL)
        {
            ft_printf("Exit");
            return 1;
        }
        if(init->line)
        {
            add_history(init->line);
        }
    }
    else
    {
        /* if the prompt doesnt come from the tty if normally comes from a file
        for ex, for that we would need to access to whatever is written in the
        file so we can use GNL to get what is written there */
        init->line = get_next_line(STDIN_FILENO);
        /*lo que pasa aca es que el shell directamente redirecciona
        el stdin del terminal al stdin del file que estamos usando*/    
    }
    if(*init->line == '\0')
    {
        free(init->line);
        return (1); // que hacemos en caso que la linea este vacia?
    }
    
    
    
    
}
