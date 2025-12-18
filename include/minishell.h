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

typedef struct s_data
{
	int interactive;
    char *line;
	int size_envp;
	char **envp;
	t_token *token_list;
}	t_data;

typedef enum s_token_type
{
	WORD, 		 	// ls, echo, file.txt
	PIPE, 			// |
	REDIR_IN,			// <
	REDIR_OUT,		// >
	REDIR_APPEND, 	// >>
	REDIR_HDOC,		// <<
	EOF,				// nomo input to read
}	t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
	struct t_token *next;
}	t_token;

void readline_calling(char **line);
int is_interactive(t_data *data);
int count_envp(char **envp);
char **copy_envp(char **envp, int size_envp);
t_data  *initAllData(char **envp);
int startOperationalLoop(t_data *data)
void freeAllData(t_data *data);

/* tokenize */
t_token *createToken(void);
int tokenizeInput(t_data *data);


#endif
