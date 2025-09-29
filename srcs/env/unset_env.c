#include "minishell.h"

int unset_env(t_env **env, char *key)
{
    t_env *current = *env;
    t_env *prev = NULL;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0) // Key found
        {
            if (prev)
                prev->next = current->next; // Remove from the middle or end
            else
                *env = current->next; // Remove from the head

            free(current->key);
            free(current->value);
            free(current);
            return (1); // Successfully removed
        }
        prev = current;
        current = current->next;
    }
    return (0); // Key not found
}