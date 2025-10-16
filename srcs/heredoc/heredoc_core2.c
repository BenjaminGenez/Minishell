/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:12:01 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 14:12:34 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_eof_warning(const char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited "
		"by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd((char *)delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == -1
		|| write(fd, "\n", 1) == -1)
	{
		free(line);
		return (1);
	}
	return (0);
}

int	read_heredoc_lines(const char *delimiter, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			display_eof_warning(delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write_heredoc_line(pipefd[1], line) != 0)
		{
			close(pipefd[1]);
			return (1);
		}
		free(line);
	}
	close(pipefd[1]);
	return (0);
}

int	setup_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	return (0);
}

int	cleanup_heredoc_resources(int pipefd[2], int status)
{
	close(pipefd[0]);
	close(pipefd[1]);
	return (status);
}
