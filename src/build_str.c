 #include "minishell.h"

/* 
I. Parsing: From Tokens to Linked List
Transform tokens into a list of command nodes (`t_cmd`)
* Create Node: Start a new `t_cmd` at the beginning and every time a `T_PIPE` is found.
* Classify:
   * `T_WORD` → Add to `char **args`.
   * Redirection → Create a `t_redir` node with the type and filename.
* Finish: When a `T_PIPE` or the end of tokens is reached, NULL-terminate the `args` array.
*/

t_cmd *create_node()
{
	t_cmd *new_cmd;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if(!new_cmd)
		return NULL;
	return (new_cmd);
}

int count_tokens(t_token *list_tokens)
{
	t_token *current;
	int size;

	current = list_tokens;
	size = 0;
	while(current != NULL && current->type != T_PIPE)
	{
		if(current->type == T_REDIR_IN || current->type == T_REDIR_OUT || 
           current->type == T_REDIR_HDOC || current->type == T_REDIR_APPEND)
		{
			current = current->next;
			if (current != NULL)
				current = current->next;
		}
		else if (current->type == T_WORD)
		{
			size++;
			current = current->next;
		}
		else
			current = current->next;
	}
	return (size);
}

int is_expansion_empty(char **arg)
{
	int i;
	char *str;
	
	i = 0;
	str = *arg;
	while(str[i])
	{
		if((!ft_isalpha(str[i])) || (!ft_isalnum(str[i])))
			return 1;
		i++;
	}
	return 0;
}

char **args_expansion(char **args)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while(args[i])
	{
		while(args[i][j])
		{
			if(args[i][j] == '$')
			{
				char next = args[i][j + 1];
    			if (next == '?')
					// expand_exit_status();

    			else if (ft_isalpha(next) || next == '_')
        			// expand_variable();

    			else if (ft_isdigit(next))
					// For $1 for instance
			}
			j++
		}
	}
}

t_cmd *create_str(t_token *list_token)
{
	t_cmd *head_cmd;
	t_cmd *current_cmd;
	t_cmd *last_cmd;
	t_token *current_tkn;
	int size_args;

	head_cmd = create_node();
	current_cmd = head_cmd;
	current_tkn = list_token;
	size_args = count_tokens(current_tkn);
	current_cmd->args = malloc(sizeof(char *) * (size_args + 1));
	current_cmd->args[size_args] = NULL;
	while(current_tkn != NULL)
	{
		if(current_tkn->type == T_PIPE)
		{
			current_cmd->next = create_node();
			if(!current_cmd->next)
				return (NULL);
			current_cmd = current_cmd->next;
			size_args = count_tokens(current_tkn);
			current_cmd->args = malloc(sizeof(char *) * (size_args + 1));
			current_cmd->args[size_args] = NULL;
		}
		else
		{
			if (current_tkn->type == T_WORD)
			{
				current_cmd->args = 
			}

		}
		current_tkn = current_tkn->next;
	}

	



}

/*
II. Expansion (Environment Variables)
Search for the `$` symbol before Word Splitting.
* Where: Check all `args` and `t_redir->file` names.
* Quote Rules:
   * Single (`' '`): Do nothing (no expansion).
   * Double (`" "`): Expand the variable (e.g., `"$USER"` → `"juan"`).
   * No Quotes: Expand the variable.
* Special Case: Replace `$?` with the exit status of the last command.


III. Word Splitting
Break down expanded variables that were not inside double quotes.
* Why: If `VAR="ls -l"`, then `$VAR` must become two separate arguments (`ls` and `-l`).
* The Rule:
   * Inside `""`: Keep as one single string (no splitting).
   * Outside `""`: Split by spaces into separate arguments.


*/