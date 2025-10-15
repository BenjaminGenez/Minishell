/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:45:08 by user              #+#    #+#             */
/*   Updated: 2025/10/03 23:37:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_env	*create_env_node(char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

char	*create_new_env_var(const char *key, const char *value)
{
	char	*new_env_var;
	int		key_len;
	int		value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_env_var = malloc(key_len + value_len + 2);
	if (!new_env_var)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	ft_strlcpy(new_env_var, key, key_len + 1);
	new_env_var[key_len] = '=';
	ft_strlcpy(new_env_var + key_len + 1, value, value_len + 1);
	return (new_env_var);
}

int	update_env_var(t_env *env, const char *key, char *new_env_var,
		int key_len)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		if (ft_strncmp(current->value, key, key_len) == 0
			&& (current->value[key_len] == '='
				|| current->value[key_len] == '\0'))
		{
			free(current->value);
			current->value = new_env_var;
			return (1);
		}
		if (current->next == NULL)
			break ;
		current = current->next;
	}
	return (0);
}

void	set_env_var(t_env *env, const char *key, const char *value)
{
	t_env	*current;
	char	*new_env_var;

	if (!env || !key || !value)
		return ;
	new_env_var = create_new_env_var(key, value);
	if (!new_env_var)
		return ;
	if (update_env_var(env, key, new_env_var, ft_strlen(key)))
		return ;
	current = env;
	while (current->next != NULL)
		current = current->next;
	if (current)
	{
		current->next = create_env_node(new_env_var);
		free(new_env_var);
		if (!current->next)
			ft_putstr_fd("minishell: failed to create new environment node\n",
				STDERR);
	}
}
