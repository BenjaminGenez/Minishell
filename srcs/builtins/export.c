/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	display_export_error(int error_type, const char *argument)
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

int	add_env_var(const char *var_value, t_env *env_list)
{
	t_env	*new_node;
	t_env	*temp;

	if (env_list && env_list->value == NULL)
	{
		env_list->value = ft_strdup(var_value);
		return (SUCCESS);
	}
	if (!(new_node = malloc(sizeof(t_env))))
		return (-1);
	new_node->value = ft_strdup(var_value);
	while (env_list && env_list->next && env_list->next->next)
		env_list = env_list->next;
	temp = env_list->next;
	env_list->next = new_node;
	new_node->next = temp;
	return (SUCCESS);
}

char	*extract_var_name(char *dest_buffer, const char *env_string)
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
	int	is_new_var;
	int	validation_result;

	is_new_var = 0;
	if (!cmd_args[1])
	{
		print_sorted_env(secret_env);
		return (SUCCESS);
	}
	else
	{
		validation_result = is_valid_env(cmd_args[1]);
		if (cmd_args[1][0] == '=')
			validation_result = -3;
		if (validation_result <= 0)
			return (display_export_error(validation_result, cmd_args[1]));
		if (validation_result == 2)
			is_new_var = 1;
		else
			is_new_var = update_existing_var(env_list, cmd_args[1]);
		if (is_new_var == 0)
		{
			if (validation_result == 1)
				add_env_var(cmd_args[1], env_list);
			add_env_var(cmd_args[1], secret_env);
		}
	}
	return (SUCCESS);
}
