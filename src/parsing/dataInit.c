#include "minishell.h"

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
} // copy_2d_array on libft?

t_data  *init_all_data(char **envp)
{
    t_data  *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    ft_memset(data, 0, sizeof(t_data)); 
	data->interactive = isatty(STDIN_FILENO); 
	data->size_envp = count_envp(envp);
	data->envp = copy_envp(envp, data->size_envp); // data->copyenvp
	if (!data->envp)
    {
        free(data);
        return (NULL);
    }  
    return (data);
}