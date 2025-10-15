/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:55:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 22:55:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_env_str(t_env *env)
{
	char	*result;
	char	*tmp;
	size_t	total_len;
	t_env	*curr;

	total_len = 0;
	curr = env;
	while (curr && (total_len += ft_strlen(curr->value) + 1))
		curr = curr->next;
	result = ft_calloc(total_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (env)
	{
		tmp = ft_strjoin(result, env->value);
		free(result);
		result = ft_strjoin(tmp, "\n");
		free(tmp);
		env = env->next;
	}
	return (result);
}

static int	get_env_size(char **env_arr)
{
	int	idx;

	idx = 0;
	while (env_arr && env_arr[idx])
		idx++;
	return (idx);
}

static void	sort_env_arr(char **env_arr, int len)
{
	int		sorted;
	int		idx;
	char	*temp;

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

static void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	print_sorted_env(t_env *env)
{
	int		idx;
	char	**env_arr;
	char	*env_str;

	env_str = build_env_str(env);
	if (!env_str)
		return ;
	env_arr = ft_split(env_str, '\n');
	free(env_str);
	sort_env_arr(env_arr, get_env_size(env_arr));
	idx = 0;
	while (env_arr && env_arr[idx])
	{
		ft_putstr("declare -x ");
		ft_putendl(env_arr[idx]);
		idx++;
	}
	free_str_array(env_arr);
}
