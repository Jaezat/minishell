#include "minishell.h"

void print_tokens(t_minishell *data)
{
	t_token *current;

	current = data->list_tokens;
	while(current)
	{
		if(current->type == T_WORD)
			{
				printf("WORD: %s\n", current->value);
				if ((ft_strcmp(current->value, "cd") == 0))
				{
					char *arg;
					if (!(current->next))
						arg = NULL;
					else
						arg = current->next->value;
					ft_cd(data, arg);
				}
				if ((ft_strcmp(current->value, "env") == 0))
					print_env_list(data->env_list);
				if ((ft_strcmp(current->value, "pwd") == 0))
					ft_pwd();
				if ((ft_strcmp(current->value, "exit") == 0))
				{
					char *args[2];
					if (!(current->next))
						args[0] = NULL;
					else
						args[0] = current->next->value;
					if (current->next && current->next->next)
						args[1] = current->next->next->value;
					ft_exit(data, args);
				}
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

void print_ast_horizontal(t_ast *node, int is_left, int depth)
{
    int i;

    if (!node)
        return;
    print_ast_horizontal(node->right, 0, depth + 1);
    i = 0;
    while (i < depth)
    {
        printf("          "); 
        i++;
    }
    if (depth > 0)
        printf(is_left ? "└── " : "┌── ");

    if (node->type == NODE_PIPE)
        printf("[PIPE]\n");
    else if (node->type == NODE_COMMAND)
	{
	    printf("CMD(%s)", node->args ? node->args[0] : "NULL");
	    if (node->infile)
	        printf(" [IN: %s]", node->infile);
	    if (node->outfile)
	        printf(" [OUT: %s]", node->outfile);
	    printf("\n");
	}
    print_ast_horizontal(node->left, 1, depth + 1);
}

void print_ast(t_ast *node, int depth)
{
	(void)depth;

    printf("\n--- TREE STRUCT AST ---\n\n");
    print_ast_horizontal(node, 0, 0);
}