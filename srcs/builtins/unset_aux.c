/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:25:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:25:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_var_name_len(char *env_entry)
{
	size_t	len;

	len = 0;
	while (env_entry[len] && env_entry[len] != '=')
		len++;
	return (len);
}

static int	remove_first_node(t_mini *shell, t_env *current_env, char *cmd_arg)
{
	if (ft_strncmp(cmd_arg, current_env->value,
			get_var_name_len(current_env->value)) == 0)
	{
		if (current_env->next)
			shell->env = current_env->next;
		remove_env_node(shell, current_env);
		return (1);
	}
	return (0);
}

static int	remove_middle_node(t_mini *shell, t_env *current_env, char *cmd_arg)
{
	t_env	*next_node;

	if (ft_strncmp(cmd_arg, current_env->next->value,
			get_var_name_len(current_env->next->value)) == 0)
	{
		next_node = current_env->next->next;
		remove_env_node(shell, current_env->next);
		current_env->next = next_node;
		return (1);
	}
	return (0);
}

int	unset_variable(char *cmd_arg, t_mini *shell)
{
	t_env	*current_env;

	current_env = shell->env;
	if (!current_env)
		return (SUCCESS);
	if (remove_first_node(shell, current_env, cmd_arg))
		return (SUCCESS);
	while (current_env && current_env->next)
	{
		if (remove_middle_node(shell, current_env, cmd_arg))
			return (SUCCESS);
		current_env = current_env->next;
	}
	return (SUCCESS);
}
