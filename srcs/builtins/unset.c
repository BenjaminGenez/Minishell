/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t get_var_name_len(char *env_entry)
{
    size_t len;

    len = 0;
    while (env_entry[len] && env_entry[len] != '=')
        len++;
    return (len);
}

static void remove_env_node(t_mini *shell, t_env *node_to_remove)
{
    if (shell->env == node_to_remove && node_to_remove->next == NULL)
    {
        mem_free(shell->env->value);
        shell->env->value = NULL;
        shell->env->next = NULL;
        return;
    }
    mem_free(node_to_remove->value);
    mem_free(node_to_remove);
}

int ft_unset(char **cmd_args, t_mini *shell)
{
    t_env *current_env;
    t_env *next_node;

    current_env = shell->env;
    if (!(cmd_args[1]))
        return (SUCCESS);
    if (ft_strncmp(cmd_args[1], current_env->value, get_var_name_len(current_env->value)) == 0)
    {
        if (current_env->next)
            shell->env = current_env->next;
        else
            shell->env = shell->env;
        remove_env_node(shell, current_env);
        return (SUCCESS);
    }
    while (current_env && current_env->next)
    {
        if (ft_strncmp(cmd_args[1], current_env->next->value, get_var_name_len(current_env->next->value)) == 0)
        {
            next_node = current_env->next->next;
            remove_env_node(shell, current_env->next);
            current_env->next = next_node;
            return (SUCCESS);
        }
        current_env = current_env->next;
    }
    return (SUCCESS);
}