#include "minishell.h"

int set_env_value(t_env **env, char *key, char *value)
{
    t_env *current = *env;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            if (!current->value)
                return (0);
            return (1);
        }
        current = current->next;
    }

    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return (0);
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = *env;
    *env = new_node;
    return (1);
}