#include "minishell.h"

t_env *env_to_list(char **envp)
{
    t_env *head = NULL;
    t_env *new_node;
    int i = 0;

    while (envp[i])
    {
        new_node = malloc(sizeof(t_env));
        if (!new_node)
            return (NULL);                                        // Handle allocation failure
        new_node->key = ft_strdup_until(envp[i], '=');            // Extract key
        new_node->value = ft_strdup(ft_strchr(envp[i], '=') + 1); // Extract value
        new_node->next = head;
        head = new_node;
        i++;
    }
    return (head);
}