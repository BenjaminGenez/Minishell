/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:22:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_env_var(const char *var_value, t_env *env_list)
{
	t_env	*new_node;
	t_env	*temp;

	if (env_list && env_list->value == NULL)
	{
		env_list->value = ft_strdup(var_value);
		return (SUCCESS);
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (-1);
	new_node->value = ft_strdup(var_value);
	while (env_list && env_list->next && env_list->next->next)
		env_list = env_list->next;
	temp = env_list->next;
	env_list->next = new_node;
	new_node->next = temp;
	return (SUCCESS);
}

int	update_existing_var(t_env *env_list, char *new_var)
{
	char	new_name[BUFF_SIZE];
	char	existing_name[BUFF_SIZE];

	extract_var_name(new_name, new_var);
	while (env_list && env_list->next)
	{
		extract_var_name(existing_name, env_list->value);
		if (ft_strcmp(new_name, existing_name) == 0)
		{
			mem_free(env_list->value);
			env_list->value = ft_strdup(new_var);
			return (1);
		}
		env_list = env_list->next;
	}
	return (SUCCESS);
}

int	ft_export(char **cmd_args, t_env *env_list, t_env *secret_env)
{
	if (!cmd_args[1])
	{
		print_sorted_env(secret_env);
		return (SUCCESS);
	}
	return (process_export_arg(cmd_args[1], env_list, secret_env));
}
