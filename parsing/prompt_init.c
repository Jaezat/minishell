/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:44:43 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/05 17:13:16 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* [ ] Argument verification
    -Check parameters: give warning if (argc != 1)
    -Should we also already here deal with seeing if
    im able to run the program from a script or in a non interactive
    way
    Ej:
    echo "ls" > test.txt
    bash < test.txt // i should have the same behaviour
*/


#include <stdio.h> 
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef struct t_init_var
{
    int interactive;
    char *prompt;
    char *line;
}   s_init_var;


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
        init->prompt = readline("minishell$ ");
    }
    
    
    
}


