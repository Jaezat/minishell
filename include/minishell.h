#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
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
int	ft_cd(t_minishell *shell, char *args);
// char	*get_env_value(t_env *head, char *env_var);
int	ft_pwd(void);
int ft_exit(t_minishell *data, char **args);
int	ft_echo(t_minishell *data, t_token *current);







#endif
