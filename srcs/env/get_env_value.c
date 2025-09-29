#include "minishell.h"

char *get_env_value(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)  // Compare keys
            return (ft_strdup(env->value)); // Return a copy of the value
        env = env->next;
    }
    return (NULL); // Key not found
}