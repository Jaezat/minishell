#include "minishell.h"

size_t	list_len(t_env *list)
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

static char	**get_env_array(t_env *env_list)
{
	char	**env_array;
	char	*tmp;
	int		i;

	env_array = malloc(sizeof(char *) * (list_len(env_list) + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env_list)
	{
		if (env_list->value)
		{
			tmp = ft_strjoin(env_list->key, "=");
			env_array[i] = ft_strjoin(tmp, env_list->value);
			free(tmp);
			if (!env_array[i])
				return (free_2d_array(env_array), NULL);
			i++;
		}
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	handle_redirections(t_redir *redir)
{
	int	fd;
	int	flags;
	int	target_fd;

	while (redir)
	{
		if (redir->type == T_REDIR_IN)
		{
			flags = O_RDONLY;
			target_fd = STDIN_FILENO;
		}
		else
		{
			target_fd = STDOUT_FILENO;
			if (redir->type == T_REDIR_OUT)
				flags = O_WRONLY | O_CREAT | O_TRUNC;
			else
				flags = O_WRONLY | O_CREAT | O_APPEND;
		}
		fd = open(redir->file, flags, 0644);
		if (check_fd_error(fd, redir->file) == -1)
			return (-1);
		dup2(fd, target_fd);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

void	handle_child_pipes(t_cmd *cmd, int *fd, int fd_in)
{
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
}

void	run_execution(t_minishell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;
	int		exit_code;

	if (!cmd->args[0])
	{
		free_all_data(shell);
		exit(0);
	}
	if (is_builtin(cmd->args[0]))
	{
		exit_code = execute_builtin(shell, cmd);
		free_all_data(shell);
		exit(exit_code);
	}
	path = get_cmd_path(shell, cmd->args[0]);
	env = get_env_array(shell->env_list);
	if (!path)
	{
		print_path_error(cmd->args[0]);
		free_2d_array(env);
		free_all_data(shell);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("execve");
	free(path);
	free_2d_array(env);
	free_all_data(shell);
	exit(1);
}
