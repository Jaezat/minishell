#include "minishell.h"

static size_t	list_len(t_env *list)
{
	size_t i;

	i = 0;
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

static t_env	**duplicate_env_list(t_env *env)
{
	t_env	*current;
	t_env	**env_duplicate;
	int		i;

	env_duplicate = malloc(sizeof(t_env *) * (list_len(env) + 1));
	if (!env_duplicate)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		env_duplicate[i] = current;
		current = current->next;
		i++;
	}
	env_duplicate[i] = NULL;
	return (env_duplicate);
}

static void print_env_duplicate(t_env **env_duplicate)
{
	int	i;

	i = 0;
	while(env_duplicate[i])
	{
		if (env_duplicate[i]->is_exported)
		{
			printf("declare -x %s", env_duplicate[i]->key);
			if (env_duplicate[i]->value)
				printf("=\"%s\"", env_duplicate[i]->value);
			printf("\n");
		}
		i++;
	}
}

static void	order_env_duplicate(t_env **env_duplicate)
{
	t_env *tmp;
	int	i;
	int	j;
	int	len;

	len = 0;
	while (env_duplicate[len])
		len++;
	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(env_duplicate[j]->key, env_duplicate[j + 1]->key) > 0)
			{
				tmp = env_duplicate[j];
				env_duplicate[j] = env_duplicate[j + 1];
				env_duplicate[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	validate_shell_variable(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static size_t	get_key_len(char *arg, char *equal_sign)
{
	if (equal_sign)
		return (equal_sign - arg);
	return (ft_strlen(arg));
}

void	extract_key_and_value_from_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	equal_sign = ft_strchr(arg, '=');
	key_len = get_key_len(arg, equal_sign);
	*key = ft_substr(arg, 0, key_len);
	if (equal_sign)
		*value = ft_strdup(arg + key_len + 1);
	else
		*value = NULL;
}

t_env *create_env_from_key_and_value(char *key, char *value)
{
    t_env   *node;
    
    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = key;
    node->value = value;
	node->is_exported = 1;
    node->next = NULL;
    return (node);
}

static void	upsert_env_var(t_minishell *shell, char *arg)
{
	t_env   *node;
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	extract_key_and_value_from_arg(arg, &key, &value);
	node = get_env_node(shell->env_list, key);
	if (node)
	{
		if (equal_sign)
		{
			free(node->value);
			node->value = value;
		}
		else
			free(value);
		free(key);
	}
	else
	{
		node = create_env_from_key_and_value(key, value);
		add_env_node(&shell->env_list, node);
	}
}

int	ft_export(t_minishell *shell, t_token *token)
{
	t_env **env_duplicate;
	t_token	*current;

	current = token->next;
	// if there are no arguments passed to
	// export, it should print the exported
	// variables list, alphabetically
	// ordered with a "declare -x" before
	if (!current)
	{
		env_duplicate = duplicate_env_list(shell->env_list);
		order_env_duplicate(env_duplicate);
		print_env_duplicate(env_duplicate);
		free(env_duplicate);
		return (0);
	} 
	// if there are arguments, we should validate the variable
	// being passed
	while (current)
	{
		if (!validate_shell_variable(current->value))
		{
			ft_putstr_fd("bash: export: WORD: not a valid identifier", 2);
			// here we should also update the exit code of the shell to 1;
			// shell->last_exit_code = 1;
		} else {
			// upsert means update or insert, which
			// is what we should do with the new value
			upsert_env_var(shell, current->value);
		}
		current = current->next;
	}
	return (0);
}
