/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andcardo <andcardo@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 14:37:11 by andcardo          #+#    #+#             */
/*   Updated: 2026/03/08 14:37:17 by andcardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		validate_shell_variable(char *str);
t_env	*create_env_from_key_and_value(char *key, char *value);
void	extract_key_and_value_from_arg(char *arg, char **key, char **value);
void	upsert_env_var(t_minishell *shell, char *arg);
int		ft_echo(char **args);
int		ft_cd(t_minishell *shell, char **args);
int		ft_pwd(t_env *env);
int		ft_export(t_minishell *shell, char **args);
int		ft_unset(t_minishell *shell, char **args);
int		ft_env(t_env *env);
int		ft_exit(t_minishell *shell, char **args);

#endif
