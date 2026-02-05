#include "minishell.h"

void free_cmd_list(t_cmd *head)
{
    t_cmd *tmp;
    int i;

    while (head)
    {
        tmp = head->next;
        if (head->args)
        {
            i = 0;
            while (head->args[i])
                free(head->args[i++]);
            free(head->args);
        }
        // Aquí llamarías a una función similar para liberar head->redirs
        // free_redir_list(head->redirs);
        free(head);
        head = tmp;
    }
}