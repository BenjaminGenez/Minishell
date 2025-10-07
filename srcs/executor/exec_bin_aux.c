/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:29:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:29:00 by user             ###   ########.fr       */
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
	DIR				directory;
	char			*result_path;

	result_path = NULL;
	directory = opendir(dir_path);
	if (!directory)
		return (NULL);
	entry = readdir(directory);
	while (entry)
	{
		if (ft_strcmp(entry->d_name, cmd_name) == 0)
			result_path = join_path(dir_path, entry->d_name);
		entry = readdir(directory);
	}
	closedir(directory);
	return (result_path);
}
