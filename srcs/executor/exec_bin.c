/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/10 15:15:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	display_error_msg(char *cmd_path)
{
	DIR	*dir_ptr;
	int	file_fd;
	int	error_code;

	file_fd = open(cmd_path, O_WRONLY);
	dir_ptr = opendir(cmd_path);
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(cmd_path, STDERR);
	if (ft_strchr(cmd_path, '/') == NULL)
		ft_putendl_fd(": command not found", STDERR);
	else if (file_fd == -1 && dir_ptr == NULL)
		ft_putendl_fd(": No such file or directory", STDERR);
	else if (file_fd == -1 && dir_ptr != NULL)
		ft_putendl_fd(": is a directory", STDERR);
	else if (file_fd != -1 && dir_ptr == NULL)
		ft_putendl_fd(": Permission denied", STDERR);
	if (ft_strchr(cmd_path, '/') == NULL || (file_fd == -1 && dir_ptr == NULL))
		error_code = UNKNOWN_COMMAND;
	else
		error_code = IS_DIRECTORY;
	if (dir_ptr)
		closedir(dir_ptr);
	ft_close(file_fd);
	return (error_code);
}

char	*join_path(const char *dir, const char *filename)
{
	char	*temp_str;
	char	*full_path;

	temp_str = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp_str, filename);
	mem_free(temp_str);
	return (full_path);
}

char	*find_cmd_in_dir(char *dir_path, char *cmd_name)
{
	struct dirent	*entry;
	DIR			*directory;
	char		*full_path;

	directory = opendir(dir_path);
	if (directory == NULL)
		return (NULL);
	while (1)
	{
		entry = readdir(directory);
		if (entry == NULL)
			break ;
		if (ft_strcmp(entry->d_name, cmd_name) == 0)
		{
			full_path = join_path(dir_path, cmd_name);
			closedir(directory);
			return (full_path);
		}
	}
	closedir(directory);
	return (NULL);
}
static int	get_env_array_size(char **envp)
{
	int	size;
	size = 0;
	if (!envp)
		return (0);
	while (envp[size])
		size++;
	return (size);
}
int	handle_bin_child_process(char *bin_path, char **cmd_args,
						t_env *env_list, t_mini *shell)
{
	pid_t	pid;
	int		exit_status;
	char	**envp;
	(void)shell;
	envp = env_list_to_array(env_list);
	if (!envp)
	{
		ft_putstr_fd("minishell: failed to convert environment to array\n", STDERR);
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(bin_path, cmd_args, envp);
		perror("minishell: execve");
		free_env_array(envp, get_env_array_size(envp));
		exit(display_error_msg(bin_path));
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		free_env_array(envp, get_env_array_size(envp));
		return (1);
	}
	waitpid(pid, &exit_status, 0);
	free_env_array(envp, get_env_array_size(envp));
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (1);
}
int	execute_binary(char *bin_path, char **cmd_args,
					t_env *env_list, t_mini *shell)
{
	if (access(bin_path, X_OK) != 0)
	{
		int status = display_error_msg(bin_path);
		if (status != 0)
			return (status);
	}
	shell->ret = handle_bin_child_process(bin_path, cmd_args, env_list, shell);
	return (shell->ret);
}
static int	handle_path_search(char **path_dirs, char **cmd_args,
				    t_env *env_list, t_mini *shell)
{
	int		dir_idx;
	char	*cmd_path;
	dir_idx = 0;
	cmd_path = NULL;
	while (path_dirs[dir_idx] && cmd_path == NULL)
	{
		cmd_path = find_cmd_in_dir(path_dirs[dir_idx], cmd_args[0]);
		dir_idx++;
	}
	if (cmd_path != NULL)
	{
		int status = execute_binary(cmd_path, cmd_args, env_list, shell);
		free(cmd_path);
		return (status);
	}
	if (ft_strchr(cmd_args[0], '/') != NULL)
	{
		int status = execute_binary(cmd_args[0], cmd_args, env_list, shell);
		if (status != 127)
			return (status);
	}
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(cmd_args[0], STDERR);
	ft_putstr_fd(": command not found\n", STDERR);
	if (isatty(STDERR_FILENO) || isatty(STDOUT_FILENO))
	{
		write(STDERR_FILENO, "", 0);
		write(STDOUT_FILENO, "", 0);
	}
	if (isatty(STDERR_FILENO) || isatty(STDOUT_FILENO))
	{
		write(STDERR_FILENO, "", 0);
		write(STDOUT_FILENO, "", 0);
	}
	shell->ret = 127;
	return (127);
}
static char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*current;
	char	*equal_sign;
	size_t	key_len;
	if (!env_list || !key)
		return (NULL);
	key_len = ft_strlen(key);
	current = env_list;
	while (current != NULL)
	{
		if (current->value != NULL)
		{
			equal_sign = ft_strchr(current->value, '=');
			if (equal_sign && (size_t)(equal_sign - current->value) == key_len &&
				ft_strncmp(current->value, key, key_len) == 0)
			{
				return (equal_sign + 1);
			}
		}
		current = current->next;
	}
	return (NULL);
}
int	exec_bin(char **cmd_args, t_env *env_list, t_mini *shell)
{
	char	**path_dirs;
	char	*path_value;
	int		status;
	if (!cmd_args || !cmd_args[0] || !cmd_args[0][0])
		return (0);
	if (ft_strchr(cmd_args[0], '/') != NULL)
	{
		if (access(cmd_args[0], F_OK) == 0)
		{
			if (access(cmd_args[0], X_OK) == 0)
				return (execute_binary(cmd_args[0], cmd_args, env_list, shell));
			ft_putstr_fd("minishell: ", STDERR);
			ft_putstr_fd(cmd_args[0], STDERR);
			ft_putendl_fd(": Permission denied", STDERR);
			return (126);
		}
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd_args[0], STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		return (127);
	}
	path_value = get_env_value(env_list, "PATH");
	if (path_value == NULL || path_value[0] == '\0')
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd_args[0], STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		return (127);
	}
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
		return (1);
	status = handle_path_search(path_dirs, cmd_args, env_list, shell);
	free_tab(path_dirs);
	if (status == 127)
		shell->ret = status;
	return (status);
}
