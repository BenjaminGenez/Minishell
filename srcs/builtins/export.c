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

/* Forward declarations */
static int	is_valid_env(char *env_var);
static int	update_existing_var(t_env *env_list, char *new_var);
static int	add_env_var(const char *var_value, t_env *env_list);
static int	add_to_both_envs(char *cmd_arg, t_env *env_list,
				t_env *secret_env, int validation_result);
static int	process_export_arg(char *cmd_arg, t_env *env_list,
				t_env *secret_env);

static void	display_sorted_env(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		i;
	int		len;

	len = 0;
	current = env;
	while (current)
	{
		len++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (len + 1));
	if (!env_array)
		return ;
	i = 0;
	current = env;
	while (current)
	{
		env_array[i++] = current->value;
		current = current->next;
	}
	env_array[i] = NULL;
	ft_sort_string_tab(env_array, i);
	i = 0;
	while (env_array[i])
	{
		ft_putstr_fd("declare -x ", STDOUT);
		ft_putendl_fd(env_array[i], STDOUT);
		i++;
	}
	free(env_array);
}

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

static char	*extract_var_name(char *dest_buffer, const char *env_string)
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

static int	process_export_arg(char *cmd_arg,
	t_env *env_list, t_env *secret_env)
{
	int	validation_result;

	if (!cmd_arg || !*cmd_arg)
		return (display_export_error(0, cmd_arg));
	validation_result = is_valid_env(cmd_arg);
	if (validation_result <= 0)
		return (display_export_error(validation_result, cmd_arg));
	if (update_existing_var(env_list, cmd_arg) == 1
		|| update_existing_var(secret_env, cmd_arg) == 1)
		return (SUCCESS);
	return (add_to_both_envs(cmd_arg, env_list, secret_env, validation_result));
}

static int	add_env_var(const char *var_value, t_env *env_list)
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

static int	update_existing_var(t_env *env_list, char *new_var)
{
	char	new_name[BUFF_SIZE];
	char	existing_name[BUFF_SIZE];

	extract_var_name(new_name, new_var);
	while (env_list && env_list->next)
	{
		extract_var_name(existing_name, env_list->value);
		if (ft_strcmp(new_name, existing_name) == 0)
		{
			free(env_list->value);
			env_list->value = ft_strdup(new_var);
			return (1);
		}
		env_list = env_list->next;
	}
	return (SUCCESS);
}

static int	is_valid_env(char *env_var)
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

int	ft_export(char **args, t_mini *mini)
{
	int	i;
	int	error;

	error = 0;
	if (!args[1])
		display_sorted_env(mini->env);
	else
	{
		i = 1;
		while (args[i])
		{
			error = process_export_arg(args[i], mini->env, mini->secret_env);
			i++;
		}
	}
	return (error);
}
