#include "minishell.h"

int count_envp(char **envp)
{
	int i;
	i = 0;
	while(envp[i])
		i++;
	return (i);
}

t_env *create_env_from_string(char *env_str)
{
    t_env   *node;
    char    *equal_sign;
    int     key_len;
    
    equal_sign = ft_strchr(env_str, '=');
    if (!equal_sign)
        return (NULL);
    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    key_len = equal_sign - env_str;
    node->key = ft_substr(env_str, 0, key_len);
    node->value = ft_strdup(equal_sign + 1);
	node->is_exported = 1;
    node->next = NULL;
    if (!node->key || !node->value)
    {
        free(node->key);
        free(node->value);
        free(node);
        return (NULL);
    }
    return (node);
}

/* 
would be something like:
node->key   = "PATH"
node->value = "/usr/bin:/bin"
node->next  = NULL
 */

void add_env_node(t_env **head, t_env *new_node)
{
    t_env *current;
    
    if (!*head)
    {
        *head = new_node;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_node;
}

t_env *envp_to_list(char **envp)
{
    t_env   *head;
    t_env   *new_node;
    int     i;
    
    head = NULL;
    i = 0;
    while (envp[i])
    {
        new_node = create_env_from_string(envp[i]);
        if (!new_node)
        {
            free_env_list(head);
            return (NULL);
        }
        add_env_node(&head, new_node);
        i++;
    }
    return (head);
}

void free_env_list(t_env *head)
{
    t_env *current;
    t_env *next;
    
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

void print_env_list(t_env *head)
{
    t_env *current;
    int i;
    
    current = head;
    i = 0;
    while (current)
    {
        printf("[%d] %s=%s\n", i, current->key, current->value);
        current = current->next;
        i++;
    }
    printf("Total: %d variables\n", i);
}

t_minishell *init_all_data(char **envp)
{
    t_minishell *data;

    data = malloc(sizeof(t_minishell));
    if (!data)
        return (NULL);
    ft_memset(data, 0, sizeof(t_minishell)); 
    data->interactive = isatty(STDIN_FILENO);
    data->env_list = envp_to_list(envp);
    if (!data->env_list)
    {
        free(data);
        return (NULL);
    }  
    return (data);
}
