/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:23:48 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 09:23:49 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_redir	t_redir;

typedef struct s_struct
{
	t_cmd		*head_cmd;
	t_cmd		*current_cmd;
	t_token		*current_tkn;
	int			arg_index;
}	t_struct;

typedef struct s_cmd
{
	char			**args;
	int				size_args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

t_cmd			*create_node(void);
t_struct		*init_struct(t_minishell *data);
t_cmd			*create_struct(t_minishell *data);
void			add_redir_node(t_cmd *cmd, t_token_type type,
					char *expanded_file);
char			*remove_quotes(char *str);
int				handle_ambiguous_redir(char **words, char *token,
					t_minishell *data);
int				count_size_args(t_token *tkn);
int				handle_pipe(t_struct *st, t_minishell *data);
int				handle_word(t_struct *st);
int				handle_redir(t_struct *st, t_minishell *data);
int				process_token(t_struct *st, t_minishell *data);

#endif
