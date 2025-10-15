/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:45:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:45:00 by user             ###   ########.fr       */
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
	if (file_fd != -1)
		close(file_fd);
	return (error_code);
}

char	*join_path(const char *dir, const char *filename)
{
	char	*temp_str;
	char	*full_path;

	temp_str = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp_str, filename);
	free(temp_str);
	return (full_path);
}

char	*find_cmd_in_dir(char *dir_path, char *cmd_name)
{
	DIR				*dir_ptr;
	struct dirent	*entry;
	char			*cmd_path;

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

int	handle_bin_child_process(char *bin_path, char **cmd_args, t_env *env_list)
{
	char	**envp;
	int		exit_status;

	envp = env_list_to_array(env_list);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(bin_path, cmd_args, envp) == -1)
	{
		exit_status = display_error_msg(bin_path);
		ft_free_array(envp);
		exit(exit_status);
	}
	ft_free_array(envp);
	return (0);
}

int	execute_binary(char *bin_path, char **cmd_args, t_env *env_list)
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
