/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariacos <mariacos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 09:23:52 by mariacos          #+#    #+#             */
/*   Updated: 2026/03/11 18:23:48 by mariacos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

void	free_all_data(t_minishell *data);
void	free_env_list(t_env *head);
void	free_token_list(t_token *head);
void	free_cmd_list(t_cmd *head);
void	free_redir_list(t_redir *head);
void	free_struct_assistant(t_struct *st, int fail);
void	print_env_list(t_env *head);
void	print_tokens(t_minishell *data);
int		print_error_unclosed_quote(char quote);
int		print_error_syntax(char *str);

#endif
