/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:05:57 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/26 09:49:09 by mariacos         ###   ########.fr       */
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

typedef struct s_data
{
	int interactive;
    char *line;
	int size_envp;
	char **envp;
	t_token *list_tokens;
}	t_data;


void readline_calling(char **line);
int is_interactive(t_data *data);
int count_envp(char **envp);
char **copy_envp(char **envp, int size_envp);
t_data  *initAllData(char **envp);
int startOperationalLoop(t_data *data);
void freeAllData(t_data *data);

/* tokenize */
t_token *createToken(t_token_type type, char *value);
void skipSpace(char c, int *i);
void freeTokenList(t_token *head);
int checkValidationToken(char **str, t_token *head, t_token *token);
int tokenizeInput(t_data *data);
int isIndexSpaceOrOperator(char c);
char *extractWord(char *str, int *i);
t_token	*checkOperator(char *str, int *i);
void addToken(t_token **head, t_token **tail, t_token *token);
void printTokens(t_token *head);




#endif
