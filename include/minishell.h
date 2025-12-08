/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:05:57 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/08 20:45:41 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   Updated: 2025/12/05 16:03:02 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

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

typedef struct s_data
{
	int interactive;
    char *line;
	int size_envp;
	char **envp;
	char **tokens;
}	t_data;

typedef enum s_token_type
{
	T_WORD, 		 	// ls, echo, file.txt
	T_PIPE, 			// |
	T_REDIR_IN,			// <
	T_REDIR_OUT,		// >
	T_REDIR_APPEND, 	// >>
	T_REDIR_HDOC,		// <<
	T_EOF,				// nomo input to read
}	t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
}	t_token;

void readline_calling(char **line);
int is_interactive(t_data *data);
int count_envp(char **envp);
char **copy_envp(char **envp, int size_envp);
t_data  *init_data(char **envp);
int operational_loop(t_data *data);
void free_all(t_data *data);




#endif
