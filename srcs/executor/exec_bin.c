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

/* Forward declarations */
char	**env_list_to_array(t_env *env_list);
void	free_str_array(char **array);

static int	display_error_msg(char *cmd_path)
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
	if (file_fd != -1)
		close(file_fd);
	return (error_code);
}

static char	*join_path(const char *dir, const char *filename)
{
	char	*temp_str;
	char	*full_path;

	temp_str = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp_str, filename);
	free(temp_str);
	return (full_path);
}

static char	*find_cmd_in_dir(char *dir_path, char *cmd_name)
{
	DIR			*dir_ptr;
	struct dirent	*entry;
	char		*cmd_path;

	dir_ptr = opendir(dir_path);
	if (!dir_ptr)
		return (NULL);
	while (1)
	{
		entry = readdir(dir_ptr);
		if (!entry)
			break ;
		if (ft_strcmp(entry->d_name, cmd_name) == 0)
		{
			cmd_path = join_path(dir_path, cmd_name);
			closedir(dir_ptr);
			return (cmd_path);
		}
	}
	closedir(dir_ptr);
	return (NULL);
}


void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	handle_bin_child_process(char *bin_path, char **cmd_args,
		t_env *env_list)
{
	char	**envp;
	int		exit_status;

	envp = env_list_to_array(env_list);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(bin_path, cmd_args, envp) == -1)
	{
		exit_status = display_error_msg(bin_path);
		free_str_array(envp);
		exit(exit_status);
	}
	free_str_array(envp);
	return (0);
}

static int	execute_binary(char *bin_path, char **cmd_args,
		t_env *env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		handle_bin_child_process(bin_path, cmd_args, env_list);
	else if (pid < 0)
		return (1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	handle_path_search(char **path_dirs, char **cmd_args,
	t_env *env_list)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (path_dirs && path_dirs[i])
	{
		cmd_path = find_cmd_in_dir(path_dirs[i], cmd_args[0]);
		if (cmd_path)
		{
			i = execute_binary(cmd_path, cmd_args, env_list);
			free(cmd_path);
			return (i);
		}
		i++;
	}
	display_error_msg(cmd_args[0]);
	return (127);
}

static char	*get_env_value(t_env *env_list, const char *key)
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

void	exec_bin(char **args, t_mini *mini)
{
	char	**path_dirs;
	char	*path_env;
	int		exit_status;

	if (!args || !args[0] || !mini)
		exit(1);
	if (ft_strchr(args[0], '/') != NULL)
	{
		exit_status = execute_binary(args[0], args, mini->env);
		exit(exit_status);
	}
	path_env = get_env_value(mini->env, "PATH");
	if (!path_env)
	{
		display_error_msg(args[0]);
		exit(127);
	}
	path_dirs = ft_split(path_env, ':');
	exit_status = handle_path_search(path_dirs, args, mini->env);
	free_str_array(path_dirs);
	exit(exit_status);
}
