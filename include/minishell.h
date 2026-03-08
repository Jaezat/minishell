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
# include "lexer.h"
# include "data.h"
# include "expansion.h"
# include "parser.h"
# include "execution.h"
# include "builtins.h"
# include "signals.h"
# include "utils.h"

typedef struct s_token		t_token;
typedef struct s_env		t_env;
typedef struct s_minishell	t_minishell;
typedef struct s_cmd		t_cmd;
typedef struct s_redir		t_redir;
typedef struct s_struct		t_struct;
typedef struct s_expand		t_expand;

int				start_operational_loop(t_minishell *data);
void			readline_calling(char **line);
int				is_interactive(t_minishell *data);
int				parse_and_execute(t_minishell *data, t_cmd **cmds);
char			*read_complete_line(void);

#endif
