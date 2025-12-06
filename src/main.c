/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 14:25:21 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/06 17:58:28 by mariacos         ###   ########.fr       */
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
/* EXPLANATION
This function checks if what returns from isatty has STDIN connected
to the terminal or to another FD. If is connected to the terminal
we can keep what was written in a string and keep it in the history.
If not we do GNL to get what is written in that FD and save it.
WE return 0 to continue.
*/
int is_interactive(s_init_var *init)
{
    if (init->interactive == 1)
    {
        init->line = readline("\033[1;34mminishell> \033[0m");
        if(init->line == NULL)
        {
            printf("exit");
            return (1); //aca hace break;
        }
        if(*init->line)
            add_history(init->line);
    }
    else
    {
        init->line = get_next_line(STDIN_FILENO);
        if(init->line == NULL)
            return (1);
    }
    return (0);
}
int main()
{
    /* if (argc != 1)
    {
        printf("Error: minishell does not accept arguments\n");
        return (1);
    } */
   
    /* Check is being run from iterative mode or non interative mode */
    s_init_var *init = malloc(sizeof(s_init_var));
    if(!init)
        return 1;
    init->interactive = isatty(STDIN_FILENO);
    while(1)
    {
        if(is_interactive(init) == 1)
            break;
        else
        {
            printf("%s\n", init->line);
            return (1);
        }
    }
    return (0);
}

