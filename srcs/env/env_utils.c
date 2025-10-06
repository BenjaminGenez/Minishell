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

size_t	calc_env_len(t_env *node)
{
	size_t	total_len;
	t_env	*current;

	total_len = 0;
	current = node;
	while (current && current->next != NULL)
	{
		if (current->value != NULL)
			total_len += ft_strlen(current->value) + 1;
		current = current->next;
	}
	return (total_len);
}

char	*build_env_str(t_env *node)
{
	char	*result;
	char	*current_val;
	size_t	pos;
	size_t	val_idx;

	if (!(result = malloc(sizeof(char) * calc_env_len(node) + 1)))
		return (NULL);
	pos = 0;
	while (node && node->next != NULL)
	{
		if (node->value != NULL)
		{
			current_val = node->value;
			val_idx = 0;
			while (current_val[val_idx])
				result[pos++] = current_val[val_idx++];
		}
		if (node->next->next != NULL)
			result[pos++] = '\n';
		node = node->next;
	}
	result[pos] = '\0';
	return (result);
}

static t_env	*create_env_node(char *value)
{
	t_env	*node;

	if (!(node = malloc(sizeof(t_env))))
		return (NULL);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

int	setup_env_list(t_mini *shell, char **env_arr)
{
	t_env	*head;
	t_env	*current;
	int		idx;

	if (!env_arr || !env_arr[0])
		return (1);
	if (!(head = create_env_node(env_arr[0])))
		return (1);
	shell->env = head;
	current = head;
	idx = 1;
	while (env_arr[idx])
	{
		if (!(current->next = create_env_node(env_arr[idx])))
			return (1);
		current = current->next;
		idx++;
	}
	return (0);
}

int	setup_secret_env(t_mini *shell, char **env_arr)
{
	t_env	*head;
	t_env	*current;
	int		idx;

	if (!env_arr || !env_arr[0])
		return (1);
	if (!(head = create_env_node(env_arr[0])))
		return (1);
	shell->secret_env = head;
	current = head;
	idx = 1;
	while (env_arr[idx])
	{
		if (!(current->next = create_env_node(env_arr[idx])))
			return (1);
		current = current->next;
		idx++;
	}
	return (0);
}
