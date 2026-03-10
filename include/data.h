#ifndef DATA_H
# define DATA_H

typedef struct s_cmd	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				is_exported;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell
{
	char	*line;
	int		interactive;
	int		size_envp;
	int		exit_status;
	int		unclosed_quotes;
	int		hdoc_counter;
	int		is_pipeline;
	t_env	*env_list;
	t_token	*list_tokens;
	t_quote	*quotes;
	t_cmd	*cmds;
}	t_minishell;

char			**ft_split_upgrade(char const *s, char c);
int				count_envp(char **envp);
void			add_env_node(t_env **head, t_env *new_node);
t_env			*create_env_from_string(char *env_str);
t_env			*envp_to_list(char **envp);
t_minishell		*init_all_data(char **envp);

#endif
