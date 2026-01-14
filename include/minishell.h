
#ifndef MINISHELL_H
# define MINISHELL_H

/*  LIBRARIES */
# include "../libft/include/libft.h"
# include "../libft/include/get_next_line.h"
# include <stdio.h> 
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>


/*  DEFINES  */



/*  STRUCTURES  */

typedef enum s_token_type
{
	T_WORD, 		 	// ls, echo, file.txt
	T_PIPE, 			// |
	T_REDIR_IN,			// <
	T_REDIR_OUT,		// >
	T_REDIR_APPEND, 	// >>
	T_REDIR_HDOC,		// <<
	T_EOF		// nomo input to read 
}	t_token_type;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}	t_node_type;

// redir only for the tree
typedef enum s_redir_type
{
	REDIR_IN,       // 
	REDIR_OUT,      // >
	REDIR_APPEND,   // >>
	REDIR_HEREDOC   // <<
}	t_redir_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
}	t_token;

//  Redir struct for the tree
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_env
{
    char            *key;      // "PATH", "HOME", etc.
    char            *value;    // "/usr/bin:/bin", "/home/user", etc.
    struct s_env    *next;
}   t_env;

typedef struct s_data
{
	int interactive;
    char *line;
	int size_envp;
	t_env   *env_list; 
	int	unclosed_quotes;
	t_token *list_tokens;
}	t_data;


void readline_calling(char **line);
int is_interactive(t_data *data);
int count_envp(char **envp);
t_env *create_env_from_string(char *env_str);
void add_env_node(t_env **head, t_env *new_node);
t_env *envp_to_list(char **envp);
void free_env_list(t_env *head);
t_data  *init_all_data(char **envp);
int start_operational_loop(t_data *data);
void free_all_data(t_data *data);
void print_env_list(t_env *head);

/* lexer: tokenize process */
t_token *create_token(t_token_type type, char *value);
void skip_space(char c, int *i);
void free_token_list(t_token *head);
int check_validation_token(char **str, t_token *head, t_token *token);
int tokenize_input(t_data *data);
int is_index_space_or_operator(char c);
char *extract_word(char *str, int *i, t_data *data);
t_token	*check_operator(char *str, int *i);
void add_token(t_token **head, t_token **tail, t_token *token);
void print_tokens(t_token *head);
char *ft_strjoin_with_newline(char *s1, char *s2);
char *read_complete_line();
int has_unclosed_quotes(char *str);

/* parser: syntax checker */
int has_invalid_pipes(t_token *list_tokens); 
int has_invalid_redirect(t_token *list_tokens);
int check_redirec(t_token *token);
int print_error_syntax(char *str);
int check_syntax(t_token *list_tokens);

#endif
