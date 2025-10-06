/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:45:28 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:45:30 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_level(const char *level_str)
{
	int	idx;

	idx = 0;
	while (level_str[idx])
	{
		if (!(level_str[idx] >= '0' && level_str[idx] <= '9'))
			return (1);
		idx++;
	}
	return (0);
}

static int	parse_level_value(const char *level_str)
{
	int	pos;
	int	sign;
	int	result;

	pos = 0;
	sign = 1;
	result = 0;
	while (level_str[pos] && ft_isspace(level_str[pos]))
		pos++;
	if (is_invalid_level(level_str))
		return (0);
	if (level_str[pos] == '-')
		sign = -1;
	if (level_str[pos] == '-' || level_str[pos] == '+')
		pos++;
	while (level_str[pos] >= '0' && level_str[pos] <= '9')
		result = result * 10 + (level_str[pos++] - '0');
	return (result * sign);
}

void	increment_shell_level(t_env *env_list)
{
	int		current_level;
	char	var_name[BUFF_SIZE];
	char	*level_str;
	char	*current_value;

	current_value = find_env_value("SHLVL", env_list);
	if (ft_strcmp(current_value, "") == 0)
		return ;
	current_level = parse_level_value(current_value) + 1;
	mem_free(current_value);
	while (env_list && env_list->next)
	{
		extract_var_name(var_name, env_list->value);
		if (ft_strcmp("SHLVL", var_name) == 0)
		{
			mem_free(env_list->value);
			level_str = ft_itoa(current_level);
			env_list->value = ft_strjoin("SHLVL=", level_str);
			mem_free(level_str);
			return ;
		}
		env_list = env_list->next;
	}
}
