#include "libft.h"

int	ft_size_2d_array(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return (0);
	while (matrix[i])
		i++;
	return (i);
}