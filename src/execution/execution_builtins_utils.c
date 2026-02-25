#include "minishell.h"

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);

	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);

	return (0);
}

int execute_builtin(t_minishell *shell, t_cmd *cmd)
{
	char *name = cmd->args[0];

	if (ft_strcmp(name, "echo") == 0)
		return (ft_echo(cmd->args));
	if (ft_strcmp(name, "cd") == 0)
		return (ft_cd(shell, cmd->args));
	if (ft_strcmp(name, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(name, "export") == 0)
		return (ft_export(shell, cmd->args));
	if (ft_strcmp(name, "unset") == 0)
		return (ft_unset(shell, cmd->args));
	if (ft_strcmp(name, "env") == 0)
		return (ft_env(shell->env_list));
	if (ft_strcmp(name, "exit") == 0)
		return (ft_exit(shell, cmd->args));
	return (1);
}

