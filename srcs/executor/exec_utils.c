/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:50:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:50:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <unistd.h>

int	display_error_msg(char *cmd_path)
{
	struct stat	path_stat;

	if (access(cmd_path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd_path, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		return (127);
	}
	stat(cmd_path, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd_path, STDERR);
		ft_putendl_fd(": Is a directory", STDERR);
		return (126);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(cmd_path, STDERR);
		ft_putendl_fd(": Permission denied", STDERR);
		return (126);
	}
	return (0);
}

char	*join_path(const char *dir, const char *filename)
{
	char	*path;
	char	*result;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	result = ft_strjoin(path, filename);
	free(path);
	return (result);
}

char	*find_cmd_in_dir(char *dir_path, char *cmd_name)
{
	DIR					*directory;
	struct dirent		*entry;
	char				*result_path;

	result_path = NULL;
	directory = opendir(dir_path);
	if (!directory)
		return (NULL);
	entry = readdir(directory);
	while (entry != NULL)
	{
		if (ft_strcmp(entry->d_name, cmd_name) == 0)
		{
			result_path = join_path(dir_path, entry->d_name);
			break ;
		}
		entry = readdir(directory);
	}
	closedir(directory);
	return (result_path);
}
