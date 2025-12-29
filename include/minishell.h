/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:05:57 by mariacos          #+#    #+#             */
/*   Updated: 2025/12/28 23:00:28 by mariacos         ###   ########.fr       */
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
	char **envp; // change to copy of envp
	t_token *list_tokens;
}	t_data;


void readline_calling(char **line);
int is_interactive(t_data *data);
int count_envp(char **envp);
char **copy_envp(char **envp, int size_envp);
t_data  *init_all_data(char **envp);
int start_operational_loop(t_data *data);
void free_all_data(t_data *data);

/* tokenize */
t_token *create_token(t_token_type type, char *value);
void skip_space(char c, int *i);
void free_token_list(t_token *head);
int check_validation_token(char **str, t_token *head, t_token *token);
int tokenize_input(t_data *data);
int is_index_space_or_operator(char c);
char *extract_word(char *str, int *i);
t_token	*check_operator(char *str, int *i);
void add_token(t_token **head, t_token **tail, t_token *token);
void print_tokens(t_token *head);




#endif
