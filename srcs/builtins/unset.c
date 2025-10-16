/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 22:50:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 22:50:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	validate_identifier(const char *arg)
{
	int	i;

	if (!arg || !*arg || ft_isdigit(*arg))
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (!is_valid_identifier_char(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

size_t	get_var_name_len(char *env_entry)
{
	size_t	len;

	len = 0;
	while (env_entry[len] && env_entry[len] != '=')
		len++;
	return (len);
}

int	remove_first_node(t_mini *mini, t_env *current_env, char *cmd_arg)
{
	if (ft_strncmp(cmd_arg, current_env->value,
			get_var_name_len(current_env->value)) == 0)
	{
		if (current_env->next)
			mini->env = current_env->next;
		remove_env_node(mini, current_env);
		return (1);
	}
	return (0);
}

int	remove_middle_node(t_mini *mini, t_env *current_env, char *cmd_arg)
{
	t_env	*next_node;

	if (ft_strncmp(cmd_arg, current_env->next->value,
			get_var_name_len(current_env->next->value)) == 0)
	{
		next_node = current_env->next->next;
		remove_env_node(mini, current_env->next);
		current_env->next = next_node;
		return (1);
	}
	return (0);
}
