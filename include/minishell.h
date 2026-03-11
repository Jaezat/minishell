/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:23:45 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 09:23:46 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
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
