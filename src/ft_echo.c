#include "minishell.h"

static int check_flag(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	if (str[i] != 'n')
		return (0);
	i++;
	while(str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_minishell *data, t_token *current)
{
	int	flag;

	flag = 1;

	if (data && current)
	{
		while(current && check_flag(current->value))
		{
			current = current->next;
			flag = 0;
		}
		while(current)
		{
			ft_putstr_fd(current->value, 1);
			if (current->next)
				ft_putstr_fd(" ", 1);
			current = current->next;
		}
		if (flag)
			ft_putchar_fd('\n', 1);
	}
	return (1);
}
