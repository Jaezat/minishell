#ifndef DATA_H
#define DATA_H

typedef struct s_env
{
    char            *key;      // "PATH", "HOME", etc.
    char            *value;    // "/usr/bin:/bin", "/home/user", etc.
	int	is_exported;
    struct s_env    *next;
}   t_env;


typedef struct s_minishell
{
	int interactive;
    char *line;
	int size_envp;
	t_env   *env_list;
	int exit_status;
	int unclosed_quotes;
	t_token *list_tokens;
	t_quote *quotes;
	t_cmd *cmds;
	int hdoc_counter;
}	t_minishell;


int count_envp(char **envp);
t_env *create_env_from_string(char *env_str);
void add_env_node(t_env **head, t_env *new_node);
t_env *envp_to_list(char **envp);
t_minishell *init_all_data(char **envp);
char	**ft_split_upgrade(char const *s, char c);

#endif
