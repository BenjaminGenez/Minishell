/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:43:56 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:44:34 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_env_size(char **env_arr)
{
    int idx;

    idx = 0;
    while (env_arr[idx])
        idx++;
    return (idx);
}

void sort_env_arr(char **env_arr, int len)
{
    int sorted;
    int idx;
    char *temp;

    sorted = 0;
    while (env_arr && sorted == 0)
    {
        sorted = 1;
        idx = 0;
        while (idx < len - 1)
        {
            if (ft_strcmp(env_arr[idx], env_arr[idx + 1]) > 0)
            {
                temp = env_arr[idx];
                env_arr[idx] = env_arr[idx + 1];
                env_arr[idx + 1] = temp;
                sorted = 0;
            }
            idx++;
        }
        len--;
    }
}

void print_sorted_env(t_env *env)
{
    int idx;
    char **env_arr;
    char *env_str;

    env_str = build_env_str(env);
    env_arr = ft_split(env_str, '\n');
    mem_free(env_str);
    sort_env_arr(env_arr, get_env_size(env_arr));
    idx = 0;
    while (env_arr[idx])
    {
        ft_putstr("declare -x ");
        ft_putendl(env_arr[idx]);
        idx++;
    }
    free_tab(env_arr);
}