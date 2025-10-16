/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:42:25 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:42:26 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_child(int prev_fd, int pipe_fd[2], t_token *current)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->next != NULL)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

static void	update_pipe_fds(int *prev_fd, int *pipe_fd, t_token **cmd_start,
		t_token *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->next != NULL)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
		*cmd_start = current->next;
	}
}

pid_t	process_pipe_segment(t_mini *shell, t_token **cmd_start,
		t_token *current, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	pid = -1;
	if (current->next != NULL && pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		setup_pipe_child(*prev_fd, pipe_fd, current);
		exec_cmd(shell, *cmd_start);
		exit(shell->ret);
	}
	update_pipe_fds(prev_fd, pipe_fd, cmd_start, current);
	return (pid);
}
