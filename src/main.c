#include "minishell.h"

void free_all_data(t_minishell *data)
{
	if(!data)
		return ;
	if(data->line)
		free(data->line);
	free(data);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell  *data;
    
    (void)argv;
    if (argc != 1)
    {
        ft_putstr_fd("Error: minishell does not accept arguments\n", 2);
        return (1);
    }
    data = init_all_data(envp);
    if (!data)
    {
        return (1);
    }
	// print_env_list(data->env_list);
    start_operational_loop(data);
    //tengo que decidir que hacer aca en caso que retorne 1
    // retornar en Stderr 
    free_all_data(data);
    return (0);
}
