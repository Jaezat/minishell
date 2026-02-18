#ifndef EXPANSION_H
#define EXPANSION_H


typedef enum s_expand_type
{
	E_NONE,
	E_STATUS,
	E_VAR,
	E_DIGIT
} t_expand_type;


typedef struct s_expand
{
    char    *res;
    int     i;
    int     single_quote;
    int     double_quote;
}   t_expand;


t_expand *init_expand(void);
void update_quotes(char c, t_expand *exp);
char *handle_dollar(char *str, t_expand *exp, t_minishell *data);
char *check_after_dollar_sign(char *str, t_minishell *data);
char **args_expansion(char **args, t_minishell *data);
char *get_var_name(char *str);
char *get_env_value_struct(char *name, t_minishell *data);
char *join_char(char *res, char c);
char *expand_exit_status(char *res, t_minishell *data);
char *expand_variable(char *res, char *str, int *i, t_minishell *data);
void process_hdoc_line(char *line, int fd, int expand, t_minishell *data);
char *manage_hdoc(char *raw_delim, t_minishell *data);









#endif