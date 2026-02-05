
#ifndef MINISHELL_H
# define MINISHELL_H

/*  LIBRARIES */
# include "../libft/include/libft.h"
# include "../libft/include/get_next_line.h"
# include <stdio.h> 
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
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

typedef enum s_expand_type
{
	E_NONE,
	E_STATUS,
	E_VAR,
	E_DIGIT
} t_expand_type;

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
	int				is_exported; // 0 for false 1 for true
    struct s_env    *next;
}   t_env;

typedef struct s_quotes
{
	int single_quote;
	int double_quote;
}	t_quote;

typedef struct s_minishell
{
	int interactive;
    char *line;
	int size_envp;
	t_env   *env_list;
	int exit_status;
	int unclosed_quotes;
	t_token *list_tokens;
	t_quote *quotes;
}	t_minishell;

/* linked list structures */

typedef struct s_redir
{
	t_token_type type;
	char *file;
	struct s_redir *next;
}	t_redir;

typedef struct s_cmd
{
	char **args;
	int size_args;
	t_redir *redirs;
	struct s_cmd *next;
} t_cmd;

typedef struct s_struct
{
    t_cmd   *head_cmd;
    t_cmd   *current_cmd;
    t_token *current_tkn;
    int     arg_index;
}   t_struct;

typedef struct s_expand
{
    char    *res;
    int     i;
    int     single_quote;
    int     double_quote;
}   t_expand;


/************** Lexer *****************/

/* init all */
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

/* tokenize process */
t_token *create_token(t_token_type type, char *value);
void skip_space(char *str, int *i);
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

/* syntax checker */
int has_invalid_pipes(t_token *list_tokens); 
int has_invalid_redirect(t_token *list_tokens);
int check_redirec(t_token *token);
int print_error_syntax(char *str);
int check_syntax(t_token *list_tokens);

/************** Parser *****************/

/* struct_expansion_utils */
t_expand *init_expand(void);
void update_quotes(char c, t_expand *exp);
char *handle_dollar(char *str, t_expand *exp, t_minishell *data);
char *check_after_dollar_sign(char *str, t_minishell *data);
char **args_expansion(char **args, t_minishell *data);

/* struct_expansion_utils */
char *get_var_name(char *str);
char *get_env_value_struct(char *name, t_minishell *data);
char *join_char(char *res, char c);
char *expand_exit_status(char *res, t_minishell *data);
char *expand_variable(char *res, char *str, int *i, t_minishell *data);

/* struct_handlers */
int count_size_args(t_token *tkn);
int  handle_pipe(t_struct *st, t_minishell *data);
int  handle_word(t_struct *st);
int  handle_redir(t_struct *st, t_minishell *data);
int  process_token(t_struct *st, t_minishell *data);

/* struct_hdoc */
void process_hdoc_line(char *line, int fd, int expand, t_minishell *data);
char *manage_hdoc(char *raw_delim, t_minishell *data);

/* struct_init */
t_cmd *create_node(void);
t_struct *init_struct(t_minishell *data);
t_cmd *create_struct(t_minishell *data);


/************** Execution *****************/

/* execution */
int	ft_cd(t_minishell *shell, char *args);
char	*get_env_value(t_env *head, char *env_var);
t_env	*get_env_node(t_env *head, char *env_var);
int	validate_shell_variable(char *str);
void	extract_key_and_value_from_arg(char *arg, char **key, char **value);
int	ft_pwd(void);
int ft_exit(t_minishell *data, char **args);
int	ft_echo(t_minishell *data, t_token *current);
int	ft_export(t_minishell *data, t_token *current);
int	ft_unset(t_minishell *shell, t_token *current);
void	handle_signals(void);
extern volatile sig_atomic_t	g_signal_status;


#endif
