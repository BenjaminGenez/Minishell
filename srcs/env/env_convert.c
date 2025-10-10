/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:50:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:50:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
int	env_list_size(t_env *env_list)
{
	int	count;
	count = 0;
	while (env_list != NULL)
	{
		if (env_list->value != NULL)
			count++;
		env_list = env_list->next;
	}
	return (count);
}
void	free_env_array(char **env_array, int size)
{
	int	i;
	i = 0;
	while (i < size)
	{
		if (env_array[i] != NULL)
			free(env_array[i]);
		i++;
	}
	free(env_array);
}
static int	is_valid_env_format(const char *env_str)
{
	const char *ptr;
	if (!env_str || !*env_str)
		return (0);
	if (!ft_isalpha(*env_str) && *env_str != '_')
		return (0);
	ptr = env_str + 1;
	while (*ptr && *ptr != '=')
	{
		if (!ft_isalnum(*ptr) && *ptr != '_')
			return (0);
		ptr++;
	}
	if (*ptr != '=' || *(ptr + 1) == '\0')
		return (0);
	return (1);
}
static int	copy_env_vars(t_env *current, char **env_array, int env_count)
{
	int	i;
	i = 0;
	while (current != NULL && i < env_count)
	{
		if (current->value != NULL && is_valid_env_format(current->value))
		{
			env_array[i] = ft_strdup(current->value);
			if (!env_array[i])
			{
				free_env_array(env_array, i);
				return (0);
			}
			i++;
		}
		current = current->next;
	}
	return (1);
}
char	**env_list_to_array(t_env *env_list)
{
	char	**env_array;
	int		env_count;
	env_count = env_list_size(env_list);
	env_array = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!env_array)
		return (NULL);
	if (!copy_env_vars(env_list, env_array, env_count))
		return (NULL);
	env_array[env_count] = NULL;
	return (env_array);
}
