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

void	set_env_var(t_env *env, const char *key, const char *value)
{
	t_env	*current;
	char	*new_env_var;
	int		key_len;
	int		value_len;

	if (!env || !key || !value)
		return ;
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_env_var = malloc(key_len + value_len + 2);
	if (!new_env_var)
	{
		perror("minishell: malloc");
		return ;
	}
	ft_strlcpy(new_env_var, key, key_len + 1);
	new_env_var[key_len] = '=';
	ft_strlcpy(new_env_var + key_len + 1, value, value_len + 1);
	current = env;
	while (current != NULL)
	{
		if (ft_strncmp(current->value, key, key_len) == 0
			&& (current->value[key_len] == '='
				|| current->value[key_len] == '\0'))
		{
			free(current->value);
			current->value = new_env_var;
			return ;
		}
		if (current->next == NULL)
			break ;
		current = current->next;
	}
	if (current)
	{
		current->next = create_env_node(new_env_var);
		free(new_env_var);
		if (!current->next)
			ft_putstr_fd("minishell: failed to create new environment node\n",
				STDERR);
	}
}

size_t	calc_env_len(t_env *node)
{
	size_t	len;

	len = 0;
	if (!node)
		return (0);
	while (node != NULL)
	{
		if (node->value != NULL)
			len += ft_strlen(node->value);
		if (node->next != NULL)
			len++;
		node = node->next;
	}
	return (len);
}

char	*build_env_str(t_env *node)
{
	size_t	len;
	char	*result;
	size_t	pos;
	size_t	i;

	if (!node)
		return (NULL);
	len = calc_env_len(node);
	if (len == 0)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	pos = 0;
	while (node != NULL)
	{
		if (node->value != NULL)
		{
			i = 0;
			while (node->value[i])
				result[pos++] = node->value[i++];
			if (node->next != NULL)
				result[pos++] = '\n';
		}
		node = node->next;
	}
	result[pos] = '\0';
	return (result);
}

int	setup_env_list(t_mini *shell, char **env_arr)
{
	t_env	*head;
	t_env	*current;
	int		idx;
	char	*path;
	char	cwd[PATH_MAX];

	if (!env_arr || !env_arr[0])
	{
		path = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
		shell->env = create_env_node(path);
		if (!shell->env)
			return (1);
		if (getcwd(cwd, sizeof(cwd)))
		{
			set_env_var(shell->env, "PWD", cwd);
			set_env_var(shell->env, "HOME", cwd);
		}
		set_env_var(shell->env, "SHLVL", "1");
		return (0);
	}
	head = create_env_node(env_arr[0]);
	if (!head)
		return (1);
	shell->env = head;
	current = head;
	idx = 1;
	while (env_arr[idx])
	{
		current->next = create_env_node(env_arr[idx]);
		if (!current->next)
			return (1);
		current = current->next;
		idx++;
	}
	if (get_env_path(shell->env, "PATH", 4) == NULL)
	{
		path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
		set_env_var(shell->env, "PATH", path);
	}
	if (get_env_path(shell->env, "PWD", 3) == NULL)
	{
		if (getcwd(cwd, sizeof(cwd)))
			set_env_var(shell->env, "PWD", cwd);
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
	head = create_env_node(env_arr[0]);
	if (!head)
		return (1);
	shell->secret_env = head;
	current = head;
	idx = 1;
	while (env_arr[idx])
	{
		current->next = create_env_node(env_arr[idx]);
		if (!current->next)
			return (1);
		current = current->next;
		idx++;
	}
	return (0);
}
