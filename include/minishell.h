
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


typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
}	t_token;

typedef struct s_env
{
    char            *key;      // "PATH", "HOME", etc.
    char            *value;    // "/usr/bin:/bin", "/home/user", etc.
    struct s_env    *next;
}   t_env;

typedef struct s_minishell
{
	int interactive;
    char *line;
	int size_envp;
	t_env   *env_list; 
	int	unclosed_quotes;
	t_token *list_tokens;
}	t_minishell;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}	t_node_type;

typedef struct s_ast
{
	t_node_type type;
	char **args;
	char *infile;
	char *outfile;
	char *heredoc;
	int append;
	struct s_ast *left;
	struct s_ast *right;
}	t_ast;


void readline_calling(char **line);
int is_interactive(t_minishell *data);
int count_envp(char **envp);
t_env *create_env_from_string(char *env_str);
void add_env_node(t_env **head, t_env *new_node);
t_env *envp_to_list(char **envp);
void free_env_list(t_env *head);
t_minishell  *init_all_data(char **envp);
int start_operational_loop(t_minishell *data);
void free_all_data(t_minishell *data);
void print_env_list(t_env *head);

/* lexer: tokenize process */
t_token *create_token(t_token_type type, char *value);
void skip_space(char c, int *i);
void free_token_list(t_token *head);
int check_validation_token(char **str, t_token *head, t_token *token);
int tokenize_input(t_minishell *data);
int is_index_space_or_operator(char c);
char *extract_word(char *str, int *i, t_minishell *data);
t_token	*check_operator(char *str, int *i);
void add_token(t_token **head, t_token **tail, t_token *token);
void print_tokens(t_minishell *data);
char *ft_strjoin_with_newline(char *s1, char *s2);
char *read_complete_line();
int has_unclosed_quotes(char *str);

/* parser: syntax checker */
int has_invalid_pipes(t_token *list_tokens); 
int has_invalid_redirect(t_token *list_tokens);
int check_redirec(t_token *token);
int print_error_syntax(char *str);
int check_syntax(t_token *list_tokens);

/* parser: ast */
int is_there_pipe(t_token *token_list);
int count_word_tokens(t_token *tokens);
t_ast *create_simple_cmd(t_token *token_list);
t_token *find_last_pipe(t_token *token_list, t_token **prev_out);
t_ast *build_ast(t_token *token_list);
void print_ast_horizontal(t_ast *node, int is_left, int depth);
void print_ast(t_ast *node, int depth);


/* execution */
int	ft_cd(t_minishell *shell, char *args);
char	*get_env_value(t_env *head, char *env_var);
int	ft_pwd(void);
int ft_exit(t_minishell *data, char **args);


#endif
