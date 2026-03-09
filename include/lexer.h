#ifndef LEXER_H
# define LEXER_H

typedef struct s_minishell	t_minishell;

typedef enum s_token_type
{
	T_WORD,			
	T_REDIR_IN,		
	T_REDIR_OUT,		
	T_REDIR_APPEND,	
	T_REDIR_HDOC,	
	T_PIPE,	
	T_EOF,			
	T_ERROR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_quotes
{
	int	single_quote;
	int	double_quote;
}	t_quote;

typedef struct s_input
{
	int		i;
	char	*word;
	t_token	*token;
	t_token	*head;
	t_token	*tail;
}	t_input;

t_token			*create_token(t_token_type type, char *value);
void			skip_space(char *str, int *i);
int				is_index_space_or_operator(char c);
int				check_validation_token(char **str, t_token *head,
					t_token *token);
char			*extract_word(char *str, int *i, t_minishell *data);
void			add_token(t_token **head, t_token **tail, t_token *token);
t_token			*check_operator(char *str, int *i);
int				tokenize_input(t_minishell *data);
int				check_redirec(t_token *token);
int				check_syntax(t_token *list_tokens);

#endif
