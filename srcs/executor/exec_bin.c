/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:55:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** This file contains the main binary execution functionality.
** It handles finding and executing external commands.
*/

int	handle_bin_child_process(char *bin_path, char **cmd_args,
						t_env *env_list, t_mini *shell)
{
	pid_t	pid;
	int		exit_status;

	(void)shell;
	pid = fork();
	if (pid == 0)
	{
		execve(bin_path, cmd_args, env_list_to_array(env_list));
		exit(display_error_msg(bin_path));
	}
	else if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	waitpid(pid, &exit_status, 0);
	return (WEXITSTATUS(exit_status));
}

int	execute_binary(char *bin_path, char **cmd_args,
					t_env *env_list, t_mini *shell)
{
	int	status;

	status = display_error_msg(bin_path);
	if (status != 0)
		return (status);
	if (access(bin_path, X_OK) == 0)
	{
		shell->ret = handle_bin_child_process(bin_path,
				cmd_args, env_list, shell);
		return (shell->ret);
	}
	return (0);
}

static int	handle_path_search(char **path_dirs, char **cmd_args,
					t_env *env_list, t_mini *shell)
{
	int		dir_idx;
	char	*cmd_path;

	dir_idx = 1;
	cmd_path = find_cmd_in_dir(path_dirs[0] + 5, cmd_args[0]);
	while (cmd_args[0] && path_dirs[dir_idx] && cmd_path == NULL)
	{
		cmd_path = find_cmd_in_dir(path_dirs[dir_idx], cmd_args[0]);
		dir_idx++;
	}
	if (cmd_path != NULL)
		return (execute_binary(cmd_path, cmd_args, env_list, shell));
	return (execute_binary(cmd_args[0], cmd_args, env_list, shell));
}

int	exec_bin(char **cmd_args, t_env *env_list, t_mini *shell)
{
	char	**path_dirs;
	t_env	*env_ptr;

	env_ptr = env_list;
	while (env_ptr && env_ptr->value
		&& ft_strncmp(env_ptr->value, "PATH=", 5) != 0)
		env_ptr = env_ptr->next;
	if (env_ptr == NULL || env_ptr->next == NULL)
		return (execute_binary(cmd_args[0], cmd_args, env_list, shell));
	path_dirs = ft_split(env_ptr->value, ':');
	if (!cmd_args[0] && !path_dirs[0])
	{
		free_tab(path_dirs);
		return (ERROR);
	}
	return (handle_path_search(path_dirs, cmd_args, env_list, shell));
}
