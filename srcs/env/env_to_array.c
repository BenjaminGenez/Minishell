#include "minishell.h"

char **env_to_array(t_env *env_list)
{
    char **envp;
    char *entry;
    int count = 0;
    t_env *tmp = env_list;

    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);

    count = 0;
    while (env_list)
    {
        entry = ft_strjoin3(env_list->key, "=", env_list->value);
        if (!entry)
        {
            ft_free_array(envp);
            return (NULL);
        }
        envp[count++] = entry;
        env_list = env_list->next;
    }
    envp[count] = NULL;
    return (envp);
}