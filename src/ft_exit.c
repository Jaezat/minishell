#include "minishell.h"

int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while(str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int ft_exit(t_minishell *data, char **args)
{
	long long final_code;

	// Always print exit
	ft_putstr_fd("exit\n", 2);

	// PATH 1: No Args
	if (!args[0])
	{
		//final_code = data->last_exit_code;
		//we need to pass the last_exit_code when no args
		//are given to exit
		final_code = 0;
		free_all_data(data);
		exit(final_code);
	}

	// PATH 2: Non-Numeric
	if (!ft_is_numeric(args[0]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		free_all_data(data);
		exit(255);
	}

	// PATH 3: Too Many Args
	if (args[1]) // Check if a 2nd arg exists
	{
		// it actually doesn't exit if there is more than 1 arg
		printf("arg[1] = %s\n", args[1]);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}

	// PATH 4: Success
	final_code = ft_atoll(args[0]);
	free_all_data(data);
	exit(final_code % 256);
}
