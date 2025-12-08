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

void free_all(t_data *data)
{
	if(!data)
		return ;
	if(data->line)
		free(data->line);
	if(data->envp)
		free_2d_array(data->envp);
	free(data);
		
}


int main(int argc, char **argv, char **envp)
{
    t_data  *data;
    
    (void)argv;
    if (argc != 1)
    {
        ft_putstr_fd("Error: minishell does not accept arguments\n", 2);
        return (1);
    }
    data = init_data(envp);
    if (!data)
        return (1);
    if (operational_loop(data) == 1)
    {
        free_all(data);
        return 1;
    }
    free_all(data);
    return (0);
}
