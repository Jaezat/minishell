#include "minishell.h"

int is_there_pipe(t_token *token_list)
{
	t_token *current;

	current = token_list;
	while(current != NULL)
	{
		if(current->type == T_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

int count_word_tokens(t_token *tokens)
{
	int size;
	t_token *current;

	size = 0;
	current = tokens;
	while(current != NULL)
	{
		if(current->type ==  T_WORD)
			size++;	
		current = current->next;
	}
	return (size);
}

t_ast *create_simple_cmd(t_token *token_list)
{
	int size_args;
	t_ast *node;
	t_token *current;
	int i;

	node = calloc(1, sizeof(t_ast));
	if(!node)
		return (NULL);
	size_args = count_word_tokens(token_list);
	node->args = malloc(sizeof(char *) * (size_args + 1));
	if(!node->args)
	{
		free(node);
		return (NULL);
	}
	current = token_list;
	i = 0;
	while(current != NULL)
	{
		if (current->type == T_WORD)
		{
			node->args[i] = ft_strdup(current->value);
			i++;
		}
		else if (current->type == T_REDIR_IN)
		{
			current = current->next;
			if (current && current->type == T_WORD)
				node->infile = ft_strdup(current->value);
		}
		else if (current->type == T_REDIR_OUT)
		{
			current = current->next;
			if (current && current->type == T_WORD)
				node->outfile = ft_strdup(current->value);
		}
		else if (current->type == T_REDIR_APPEND)
		{
			current = current->next;
			if (current && current->type == T_WORD)
			{
				node->outfile = ft_strdup(current->value);
				node->append = 1;
			}
		}
		else if (current->type == T_REDIR_HDOC) // > 
		{
			current = current->next;
			if (current && current->type == T_WORD)
				node->heredoc = ft_strdup(current->value);
		}
		current = current->next;
	}
	node->args[i] = NULL;

	return (node);
}

t_token *find_last_pipe(t_token *token_list, t_token **prev_out)
{
	t_token *prev_to_pipe;
	t_token *prev;
	t_token *current;
	t_token *last_pipe;

	prev = NULL;
	last_pipe = NULL;
	prev_to_pipe = NULL;
	current = token_list;

	while(current != NULL)
	{
		if(current->type == T_PIPE)
		{
			last_pipe = current;
			prev_to_pipe = prev;
		}
		prev = current;
		current = current->next;
	}
	*prev_out = prev_to_pipe;
	return (last_pipe);
}

t_ast *build_ast(t_token *token_list)
{
    t_token *last_pipe;
    t_token *prev_to_pipe;
    t_token *left_tokens;
    t_token *right_tokens;
    t_ast   *node;

    if (!token_list)
        return (NULL);
    if (is_there_pipe(token_list) == 0)
        return (create_simple_cmd(token_list));    
    last_pipe = find_last_pipe(token_list, &prev_to_pipe);
    node = ft_calloc(1, sizeof(t_ast));
    if (!node)
        return (NULL);
    node->type = NODE_PIPE;
    left_tokens = token_list;           
    right_tokens = last_pipe->next;     
    if (prev_to_pipe)
        prev_to_pipe->next = NULL;      
    else
        left_tokens = NULL;             
    node->left = build_ast(left_tokens);
    node->right = build_ast(right_tokens);
    if (last_pipe)
    {
        if (last_pipe->value)
            free(last_pipe->value);
        free(last_pipe);
    }
    return (node);
}
