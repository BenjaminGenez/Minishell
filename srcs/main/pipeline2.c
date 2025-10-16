/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:00:53 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 14:01:16 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_all_children(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	while (wait(NULL) > 0)
		;
	g_signal.pid = 0;
}

static pid_t	process_pipeline_loop(t_mini *shell, int *prev_fd)
{
	t_token	*cmd_start;
	t_token	*current;
	pid_t	last_pid;

	last_pid = -1;
	cmd_start = shell->start;
	current = shell->start;
	while (current)
	{
		if (current->type == PIPE || current->next == NULL)
		{
			last_pid = process_pipe_segment(shell, &cmd_start, current,
					prev_fd);
			if (last_pid == -1)
				return (-1);
			if (current->next == NULL)
				g_signal.pid = last_pid;
		}
		current = current->next;
	}
	return (last_pid);
}

void	exec_pipeline(t_mini *shell)
{
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = process_pipeline_loop(shell, &prev_fd);
	if (last_pid == -1)
		return ;
	wait_all_children(prev_fd);
}
