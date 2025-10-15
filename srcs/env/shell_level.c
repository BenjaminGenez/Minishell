/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 23:05:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 23:05:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(char *dest, const char *src);
static char	*find_env_value_local(const char *var_name, t_env *env_list);

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
	if (level_str[pos] == '-')
		sign = -1;
	if (level_str[pos] == '-' || level_str[pos] == '+')
		pos++;
	if (!(level_str[pos] >= '0' && level_str[pos] <= '9'))
		return (0);
	while (level_str[pos] >= '0' && level_str[pos] <= '9')
		result = result * 10 + (level_str[pos++] - '0');
	return (result * sign);
}

static char	*extract_var_name(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '=' && i < BUFF_SIZE - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*find_env_value_local(const char *var_name, t_env *env_list)
{
	char	var_name_buf[BUFF_SIZE];
	char	*value;

	if (!var_name || !env_list)
		return (ft_strdup(""));
	while (env_list)
	{
		extract_var_name(var_name_buf, env_list->value);
		if (ft_strcmp(var_name_buf, var_name) == 0)
		{
			value = ft_strchr(env_list->value, '=');
			if (value)
				return (ft_strdup(value + 1));
			return (ft_strdup(""));
		}
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}

static int	update_shlvl_value(t_env *env_list, int current_level)
{
	char	var_name[BUFF_SIZE];
	char	*level_str;

	while (env_list && env_list->next)
	{
		extract_var_name(var_name, env_list->value);
		if (ft_strcmp("SHLVL", var_name) == 0)
		{
			free(env_list->value);
			level_str = ft_itoa(current_level);
			env_list->value = ft_strjoin("SHLVL=", level_str);
			free(level_str);
			return (1);
		}
		env_list = env_list->next;
	}
	return (0);
}

void	increment_shell_level(t_env *env_list)
{
	int		current_level;
	char	*current_value;

	current_value = find_env_value_local("SHLVL", env_list);
	if (ft_strcmp(current_value, "") == 0)
	{
		free(current_value);
		return ;
	}
	current_level = parse_level_value(current_value) + 1;
	free(current_value);
	update_shlvl_value(env_list, current_level);
}
