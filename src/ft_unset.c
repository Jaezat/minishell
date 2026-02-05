#include "minishell.h"

void	remove_node_from_list(t_minishell *shell, t_env *target_node)
{
	t_env **head;
	head = &shell->env_list;
	while (*head && *head != target_node)
		head = &(*head)->next;
	*head = target_node->next;
}

static void	remove_env_var(t_minishell *shell, char *arg)
{
	t_env   *node;
	char	*key;
	char	*value;

	extract_key_and_value_from_arg(arg, &key, &value);
	node = get_env_node(shell->env_list, key);
	if (node)
		remove_node_from_list(shell, node);
}

int	ft_unset(t_minishell *shell, t_token *token)
{
	t_token	*current;
	(void)shell;

	current = token->next;

	while (current)
	{
		if (!validate_shell_variable(current->value))
		{
			//should unset handle errors tho?
			ft_putstr_fd("bash: export: WORD: not a valid identifier", 2);
		} else {
			remove_env_var(shell, current->value);
		}
		current = current->next;
	}
	return (0);
}
