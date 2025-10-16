/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:49:54 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:49:56 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_empty_env(t_mini *shell)
{
	char	*path;
	char	cwd[PATH_MAX];

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

int	create_env_list(t_mini *shell, char **env_arr)
{
	t_env	*head;
	t_env	*current;
	int		idx;

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
	return (0);
}

int	setup_env_list(t_mini *shell, char **env_arr)
{
	char	*path;
	char	cwd[PATH_MAX];

	if (!env_arr || !env_arr[0])
		return (setup_empty_env(shell));
	if (create_env_list(shell, env_arr))
		return (1);
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
