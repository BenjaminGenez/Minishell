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

/* Forward declarations */
static int	validate_identifier(const char *arg);
static size_t	get_var_name_len(char *env_entry);
static int	remove_first_node(t_mini *mini, t_env *current_env, char *cmd_arg);
static int	remove_middle_node(t_mini *mini, t_env *current_env, char *cmd_arg);
static int	unset_variable(char *cmd_arg, t_mini *mini);
static void	remove_env_node(t_mini *mini, t_env *node_to_remove);

static int	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static int	validate_identifier(const char *arg)
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

static size_t	get_var_name_len(char *env_entry)
{
	size_t	len;

	len = 0;
	while (env_entry[len] && env_entry[len] != '=')
		len++;
	return (len);
}

static int	remove_first_node(t_mini *mini, t_env *current_env, char *cmd_arg)
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

static int	remove_middle_node(t_mini *mini, t_env *current_env, char *cmd_arg)
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

static void	remove_env_node(t_mini *mini, t_env *node_to_remove)
{
	if (mini->env == node_to_remove && node_to_remove->next == NULL)
	{
		free(mini->env->value);
		mini->env->value = NULL;
		mini->env->next = NULL;
		return ;
	}
	free(node_to_remove->value);
	free(node_to_remove);
}

static int	unset_variable(char *cmd_arg, t_mini *mini)
{
	t_env	*current_env;

	current_env = mini->env;
	if (!current_env)
		return (SUCCESS);
	if (remove_first_node(mini, current_env, cmd_arg))
		return (SUCCESS);
	while (current_env && current_env->next && current_env->next->next)
	{
		if (remove_middle_node(mini, current_env, cmd_arg))
			return (SUCCESS);
		current_env = current_env->next;
	}
	if (current_env && current_env->next && ft_strncmp(cmd_arg,
			current_env->next->value,
			get_var_name_len(current_env->next->value)) == 0)
	{
		remove_env_node(mini, current_env->next);
		current_env->next = NULL;
	}
	return (SUCCESS);
}

int	ft_unset(char **args, t_mini *mini)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!validate_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
		{
			if (unset_variable(args[i], mini) != 0)
				status = 1;
		}
		i++;
	}
	return (status);
}
