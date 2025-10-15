/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 23:10:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 23:10:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env_list, const char *key)
{
	char	*equal_sign;
	size_t	key_len;

	if (!env_list || !key)
		return (NULL);
	key_len = ft_strlen(key);
	while (env_list)
	{
		equal_sign = ft_strchr(env_list->value, '=');
		if (equal_sign && (size_t)(equal_sign - env_list->value) == key_len
			&& ft_strncmp(env_list->value, key, key_len) == 0)
			return (equal_sign + 1);
		env_list = env_list->next;
	}
	return (NULL);
}

static int	exec_with_path(char *bin_path, char **args, t_mini *mini)
{
	int	exit_status;

	exit_status = execute_binary(bin_path, args, mini->env);
	exit(exit_status);
}

static int	exec_path_search(char **args, t_mini *mini)
{
	char	**path_dirs;
	char	*path_env;
	int		exit_status;

	path_env = get_env_value(mini->env, "PATH");
	if (!path_env)
	{
		display_error_msg(args[0]);
		exit(127);
	}
	path_dirs = ft_split(path_env, ':');
	exit_status = handle_path_search(path_dirs, args, mini->env);
	ft_free_array(path_dirs);
	exit(exit_status);
}

void	exec_bin(char **args, t_mini *mini)
{
	if (!args || !args[0] || !mini)
		exit(1);
	if (ft_strchr(args[0], '/') != NULL)
		exec_with_path(args[0], args, mini);
	else
		exec_path_search(args, mini);
}

int	minipipe(t_mini *mini)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	mini->saved_stdout = dup(STDOUT_FILENO);
	if (mini->saved_stdout == -1)
	{
		perror("minishell: dup");
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		close(pipefd[1]);
		close(mini->saved_stdout);
		return (0);
	}
	close(pipefd[1]);
	mini->pipin = pipefd[0];
	mini->last = 0;
	return (1);
}
