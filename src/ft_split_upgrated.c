#include "minishell.h"

static void	update_q(char c, t_quote *q)
{
	if (c == '\'' && !q->double_quote)
		q->single_quote = !q->single_quote;
	else if (c == '\"' && !q->single_quote)
		q->double_quote = !q->double_quote;
}

static int	numofwords(char *s, char c)
{
	int		i;
	int		count;
	t_quote	q;

	i = 0;
	count = 0;
	q.single_quote = 0;
	q.double_quote = 0;
	while (s[i])
	{	
		while (s[i] && s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] && (s[i] != c || q.single_quote || q.double_quote))
		{
			update_q(s[i], &q);
			i++;
		}
	}
	return (count);
}

/* static int	allocmemwords(char **array, char *s, char c)
{
	int		i;
	int		j;
	int		len;
	t_quote	q;

	i = 0;
	j = 0;
	q.single_quote = 0;
	q.double_quote = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		len = 0;
		while (s[i + len] && (s[i + len] != c || q.single_quote || q.double_quote))
		{
			update_q(s[i + len], &q);
			len++;
		}
		array[j++] = (char *)malloc(sizeof(char) * (len + 1));
		if (!array[j - 1])
			return (1);
		i += len;
	}
	return (0);
} */

static int  allocmemwords(char **array, char *s, char c)
{
    int     i;
    int     j;
    int     start;
    t_quote q;

	i = 0;
	j = 0;
	q.single_quote = 0;
	q.double_quote = 0;
    while (s[i])
    {
        while (s[i] && s[i] == c)
            i++;
        if (s[i] == '\0')
            break ;
        start = i;
        while (s[i] && (s[i] != c || q.single_quote || q.double_quote))
        {
            update_q(s[i], &q);
            i++;
        }
        array[j++] = malloc(sizeof(char) * (i - start + 1));
        if (!array[j - 1])
            return (1);
    }
    return (0);
}

static int	addingvaluesarrays(char **array, char *s, char c)
{
	int		i;
	int		j;
	int		k;
	t_quote	q;

	i = 0;
	j = 0;
	q.single_quote = 0;
	q.double_quote = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		k = 0;
		while (s[i] && (s[i] != c || q.single_quote || q.double_quote))
		{
			update_q(s[i], &q);
			array[j][k++] = s[i++];
		}
		array[j++][k] = '\0';
	}
	return (0);
}

char	**ft_split_upgrade(char const *s, char c)
{
	char	**array_s;
	int		total;

	if (!s)
		return (NULL);
	total = numofwords((char *)s, c);
	array_s = (char **)malloc(sizeof(char *) * (total + 1));
	if (!array_s)
		return (NULL);
	array_s[total] = NULL;
	if (allocmemwords(array_s, (char *)s, c))
	{
		// free_2d_array(array_s);
		return (NULL);
	}
	addingvaluesarrays(array_s, (char *)s, c);
	return (array_s);
}

/* char    **ft_split_upgrade(char const *s, char c)
{
    char    **array_s;
    int     total;
    int     debug_i = 0;

    if (!s) return (NULL);
    total = numofwords((char *)s, c);
    
    // DEBUG 1: ¿Cuántas palabras detecta el split?
    printf("DEBUG SPLIT: total detectado = %d\n", total);

    array_s = (char **)malloc(sizeof(char *) * (total + 1));
    if (!array_s) return (NULL);
    array_s[total] = NULL;
    
    if (allocmemwords(array_s, (char *)s, c))
        return (NULL);
    addingvaluesarrays(array_s, (char *)s, c);

    // DEBUG 2: ¿Qué hay dentro del array resultante?
    while (array_s[debug_i])
    {
        printf("DEBUG SPLIT: palabra[%d] = |%s|\n", debug_i, array_s[debug_i]);
        debug_i++;
    }
    return (array_s);
} */



