/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:22:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:22:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	display_export_error(int error_type, const char *argument)
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

static int	add_to_both_envs(char *cmd_arg, t_env *env_list,
				t_env *secret_env, int validation_result)
{
	if (validation_result == 1)
		add_env_var(cmd_arg, env_list);
	add_env_var(cmd_arg, secret_env);
	return (SUCCESS);
}

int	process_export_arg(char *cmd_arg, t_env *env_list, t_env *secret_env)
{
	int	is_new_var;
	int	validation_result;

	validation_result = is_valid_env(cmd_arg);
	if (cmd_arg[0] == '=')
		validation_result = -3;
	if (validation_result <= 0)
		return (display_export_error(validation_result, cmd_arg));
	if (validation_result == 2)
		is_new_var = 1;
	else
		is_new_var = update_existing_var(env_list, cmd_arg);
	if (is_new_var == 0)
		return (add_to_both_envs(cmd_arg, env_list,
				secret_env, validation_result));
	return (SUCCESS);
}
