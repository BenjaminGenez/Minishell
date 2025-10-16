/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:44:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 22:44:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_sorted_env_print(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", STDOUT);
		ft_putendl_fd(env_array[i], STDOUT);
		i++;
	}
}

void	display_sorted_env(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		count;

	count = count_env_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return ;
	current = env;
	count = 0;
	while (current)
	{
		env_array[count++] = current->value;
		current = current->next;
	}
	env_array[count] = NULL;
	ft_sort_string_tab(env_array, count);
	display_sorted_env_print(env_array, count);
	free(env_array);
}

int	export_display_error(int error_type, const char *argument)
{
	int	idx;

	if (error_type == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error_type == 0 || error_type == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	idx = 0;
	while (argument[idx] && (argument[idx] != '=' || error_type == -3))
	{
		write(STDERR, &argument[idx], 1);
		idx++;
	}
	write(STDERR, "\n", 1);
	return (ERROR);
}

char	*export_extract_var_name(char *dest_buffer, const char *env_string)
{
	int	pos;

	pos = 0;
	while (env_string[pos] && env_string[pos]
		!= '=' && ft_strlen(env_string) < BUFF_SIZE)
	{
		dest_buffer[pos] = env_string[pos];
		pos++;
	}
	dest_buffer[pos] = '\0';
	return (dest_buffer);
}

int	export_is_valid_env(char *env_var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(env_var[i]) && env_var[i] != '_')
		return (0);
	i++;
	while (env_var[i] && env_var[i] != '=')
	{
		if (!ft_isalnum(env_var[i]) && env_var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
