#include "minishell.h"

int    print_error_unclosed_quote(char quote)
{
    ft_putstr_fd("minishell: syntax error: missing matching ", 2);
    ft_putstr_fd("'", 2);
    ft_putchar_fd(quote, 2);
    ft_putstr_fd("'", 2);
    ft_putchar_fd('\n', 2);
    return (1);
}

int	print_error_syntax(char *str)
{
	char	*err_msg;

	err_msg = "minishell: syntax error near unexpected token ";
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
	return (1);
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

void print_tokens(t_minishell *data)
{
	t_token *current;

	current = data->list_tokens;
	while(current)
	{
		if(current->type == T_WORD)
			{
				printf("WORD: %s\n", current->value);
			}
		else if(current->type == T_PIPE)
			printf("PIPE: %s\n", current->value);
		else if(current->type == T_REDIR_IN)
			printf("REDIR_IN: %s\n", current->value);
		else if(current->type == T_REDIR_OUT)
			printf("REDIR_OUT: %s\n", current->value);
		else if(current->type == T_REDIR_HDOC)
			printf("REDIR_HDOC: %s\n", current->value);
		else if(current->type == T_REDIR_APPEND)
			printf("REDIR_APPEND: %s\n", current->value);
		else
			printf("EOF: %s\n", current->value);
		current = current->next;
	}
}

void print_commands(t_cmd *head)
{
    t_cmd   *curr;
    t_redir *r;
    int     i;
    int     cmd_idx;

    if (!head)
    {
        printf("\n=== COMMAND STRUCTURE ===\n");
        printf("(NULL - no commands)\n");
        printf("==========================\n\n");
        return;
    }

    curr = head;
    cmd_idx = 0;
    printf("\n=== COMMAND STRUCTURE ===\n");
    
    while (curr)
    {
        printf("COMMAND [%d]:\n", cmd_idx++);
        
        // 1. Imprimir Argumentos
        printf("  Args: ");
        if (curr->args)
        {
            i = 0;
            while (curr->args[i])
            {
                printf("[%s] ", curr->args[i]);
                i++;
            }
            if (i == 0)
                printf("(empty array)");
        }
        else
            printf("(NULL)");
        printf("\n");

        // 2. Imprimir Redirecciones
        printf("  Redirs: ");
        r = curr->redirs;
        if (!r)
        {
            printf("(none)");
        }
        else
        {
            while (r)
            {
                printf("{Type: %d, File: %s}", r->type, r->file);
                r = r->next;
                if (r)
                    printf(" -> ");
            }
        }
        printf("\n");
        
        printf("  Size Args: %d\n", curr->size_args);
        printf("  Next: %s\n", curr->next ? "YES (has pipe)" : "NO (last command)");
        printf("--------------------------\n");
        
        curr = curr->next;
    }
    
    printf("Total commands: %d\n", cmd_idx);
    printf("==========================\n\n");
}
