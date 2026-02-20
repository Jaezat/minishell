#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/include/libft.h"

/*  Forward Declarations */
typedef struct s_token      t_token;
typedef struct s_env        t_env;
typedef struct s_minishell  t_minishell;
typedef struct s_cmd        t_cmd;
typedef struct s_redir      t_redir;
typedef struct s_struct     t_struct;
typedef struct s_expand     t_expand;

/* Header Files */
# include "lexer.h"
# include "data.h"
# include "expansion.h"
# include "parser.h"
# include "utils.h"

/* (main / operational_loop) */
int     start_operational_loop(t_minishell *data);
void    readline_calling(char **line);
int     is_interactive(t_minishell *data);


/************** Execution *****************/

/* execution */
char	*get_env_value(t_env *head, char *env_var);
t_env	*get_env_node(t_env *head, char *env_var);
void	update_env_var(t_env *head, char *env_var, char *new_value);
int	validate_shell_variable(char *str);
void	extract_key_and_value_from_arg(char *arg, char **key, char **value);
void	handle_signals(void);
char *get_cmd_path(t_minishell *shell, char *cmd);
size_t	list_len(t_env *list);
void	print_path_error(char *cmd);
void	execute_pipeline(t_minishell *shell, t_cmd *cmd_list);
void execute_commands(t_minishell *shell, t_cmd *cmd_list);
extern volatile sig_atomic_t	g_signal_status;
void handle_child_pipes(t_cmd *cmd, int *fd, int fd_in);
void run_execution(t_minishell *shell, t_cmd *cmd);
void	handle_redirections(t_redir *redir);
int is_builtin(char *cmd);
int execute_builtin(t_minishell *shell, t_cmd *cmd);
void check_fd_error(int fd, char *filename);
int	handle_heredocs(t_cmd *cmd);

/* execution builtins */
int ft_echo(char **args);
int ft_cd(t_minishell *shell, char **args);
int ft_pwd(void);
int ft_export(t_minishell *shell, char **args);
int ft_unset(t_minishell *shell, char **args);
int ft_env(t_env *env);
int ft_exit(t_minishell *shell, char **args);






#endif
