#ifndef EXECUTION_H
# define EXECUTION_H

size_t	list_len(t_env *list);
char	*get_env_value(t_env *head, char *env_var);
t_env	*get_env_node(t_env *head, char *env_var);
void	update_env_var(t_env *head, char *env_var, char *new_value);
char	*get_cmd_path(t_minishell *shell, char *cmd);
void	print_path_error(char *cmd);
void	execute_pipeline(t_minishell *shell, t_cmd *cmd_list);
void	execute_commands(t_minishell *shell, t_cmd *cmd_list);
void	handle_child_pipes(t_cmd *cmd, int *fd, int fd_in);
void	run_execution(t_minishell *shell, t_cmd *cmd);
int		handle_redirections(t_redir *redir);
int		is_builtin(char *cmd);
int		execute_builtin(t_minishell *shell, t_cmd *cmd);
int		check_fd_error(int fd, char *filename);
int		is_lone_builtin(t_cmd *cmd);
void	execute_lone_builtin(t_minishell *shell, t_cmd *cmd);
int		handle_heredocs(t_cmd *cmd, t_minishell *shell);
void	increase_shell_level(t_env *env);

#endif
