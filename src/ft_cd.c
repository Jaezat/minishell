#include "minishell.h"

char	*get_env_value(t_env *head, char *env_var)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, env_var) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	update_env_var(t_env *head, char *env_var, char *new_value)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, env_var) == 0)
		{
			if (current->value)
				free(current->value);
			current->value = ft_strdup(new_value);
			return;
		}
		current = current->next;
	}
}

int	ft_cd(t_data *shell, char *args)
{
	char	*target_path;
	// PATH_MAX is defined in limits.h as a macro
	// for the max nb of bytes in a path.
	char	new_cwd[PATH_MAX];
	char	old_cwd[PATH_MAX];

	// Redirecting ~HOME if the given path is NULL
	if (!args)
	{
		target_path = get_env_value(shell->env_list, "HOME");
		if (!target_path)
			return (perror("minishell: cd: HOME not set"), 1);
	}
	else if (ft_strcmp(args, "-") == 0) {
		target_path = get_env_value(shell->env_list, "OLDPWD");
		if (!target_path)
			return (perror("minishell: cd: OLDPWD not set"), 1);
		//On this particular case, cd prints the path
		//it is going to. So that the user remembers it.
		ft_printf("%s\n", target_path);
	}
	else
		target_path = args;

	//Save the current directory before moving in a buffer
	getcwd(old_cwd, PATH_MAX);

	// Do the actual move with chdir.
	if (chdir(target_path) == -1)
	{
		// Need to find a way to print the name of the file here...
		// maybe perror(minishell: cd: ) printf(args[1]) perror(: No such file)?
		perror("minishell: cd: name-of-file: No such file or directory");
		return (1);
	}

	// If the actual change succed, uptdate the oldpwd env var
	update_env_var(shell->env_list, "OLDPWD", old_cwd);

	// Update the PWD env var to where we actually landed
	if (getcwd(new_cwd, PATH_MAX) != NULL)
		update_env_var(shell->env_list, "PWD", new_cwd);

	return (0);
}
