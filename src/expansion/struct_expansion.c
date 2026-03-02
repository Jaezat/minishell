#include "minishell.h"

t_expand	*init_expand(void)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->i = 0;
	exp->single_quote = 0;
	exp->double_quote = 0;
	exp->res = ft_strdup("");
	if (!exp->res)
	{
		free(exp);
		return (NULL);
	}
	return (exp);
}

void	update_quotes(char c, t_expand *exp)
{
	if (c == '\'' && !exp->double_quote)
		exp->single_quote = !exp->single_quote;
	else if (c == '\"' && !exp->single_quote)
		exp->double_quote = !exp->double_quote;
}

char	*handle_dollar(char *str, t_expand *exp, t_minishell *data)
{
	if (str[exp->i + 1] == '?')
	{
		exp->res = expand_exit_status(exp->res, data);
		exp->i += 2;
		return (exp->res);
	}
	else if (ft_isalnum(str[exp->i + 1]) || str[exp->i + 1] == '_')
	{
		exp->res = expand_variable(exp->res, &str[exp->i + 1], &exp->i, data);
		exp->i++;
		return (exp->res);
	}
	return (NULL);
}

char	*check_after_dollar_sign(char *str, t_minishell *data)
{
	t_expand	*exp;
	char		*tmp;
	char		*result;

	exp = init_expand();
	if (!exp)
		return (NULL);
	while (str[exp->i])
	{
		update_quotes(str[exp->i], exp);
		if (str[exp->i] == '$' && str[exp->i + 1] && !exp->single_quote)
		{
			if (handle_dollar(str, exp, data))
				continue ;
		}
		tmp = exp->res;
		exp->res = join_char(tmp, str[exp->i]);
		exp->i++;
	}
	result = exp->res;
	free(exp);
	return (result);
}

/* char	**args_expansion(char **args, t_minishell *data)
{
	int		i;
	int		j;
	char	*new_str;
	char	*clean_str;

	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		new_str = check_after_dollar_sign(args[i], data);
		clean_str = remove_quotes(new_str);
		free(new_str);
		free(args[i]);
		if (clean_str && clean_str[0] == '\0')
			free(clean_str);
		else
			args[j++] = clean_str;
		i++;
	}
	while (j <= i)
		args[j++] = NULL;
	return (args);
} */

char	**args_expansion(char **args, t_minishell *data)
{
	int		i;
	int		j;
	char	*new_str;
	char	*clean_str;

	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	/* while (args[i])
	{
		new_str = check_after_dollar_sign(args[i], data);
		clean_str = remove_quotes(new_str);
		free(new_str);
		free(args[i]);
		if (clean_str)
			args[j++] = clean_str;
		i++;
	} */
	while (args[i])
    {
        new_str = check_after_dollar_sign(args[i], data);
        clean_str = remove_quotes(new_str);
        free(new_str);

        if (i != j) // If i and j are the same, we already freed it above
        {
            free(args[j]); 
        }
        free(args[i]);
        if (clean_str)
            args[j++] = clean_str;
        i++;
    }
	while (j < i)
	{
    	if (args[j])
    	    free(args[j]);
    	args[j++] = NULL;
	}
	if (args[0] && args[0][0] == '\0' && j == 1)
	{
		free(args[0]);
		args[0] = NULL;
	}
	return (args);
}
