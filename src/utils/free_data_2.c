#include "minishell.h"

void	free_token_list(t_token *head)
{
	t_token	*temp;

	while (head != NULL)
	{
		temp = head->next;
		free(head->value);
		free(head);
		head = temp;
	}
}

void	free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

