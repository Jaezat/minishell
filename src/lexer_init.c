#include "minishell.h"

t_token *createToken(t_token_type type, char *value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = 
	
    
}

void skipSpace(char c, int *i)
{
	while(c == ' ' || c == '\t')
		*i++;
}

void assignValueToken(t_token *token, char *value)
{

}

int tokenizeInput(t_data *data)
{
	int i;
	char *str;
	t_token *token;

	i = 0;
	str = ft_strdup(data->line);
	while(str[i])
	{
		token = create_token();
		if(!token)
			return (1);
		skipSpace(str[i], i);
		if(str[i] == '>' && str[i+1] == '>')
		{

		}
	}
	free(str);
}







