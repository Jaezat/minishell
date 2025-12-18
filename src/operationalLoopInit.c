#include "minishell.h"


void readline_calling(char **line)
{
    *line = readline(
        "\001\033[1;35m\002minishell$>\001\033[0m\002 "
    );
}

int is_interactive(t_data *data)
{
    readline_calling(&(data->line));
    if (data->line == NULL)
    {
        ft_printf("Exit");
        return 1;
    }
    if (data->line)
    {
        add_history(data->line);
    }
    return (0);
}
int startOperationaLoop(t_data *data)
{
    while (1)
    {
        if (is_interactive(data) == 1)
            return 1;
    }
    printf("here");
    printf("%s\n", data->line); 
    return 0;
}