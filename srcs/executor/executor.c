/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 23:30:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 23:30:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_fd(int fd)
{
	return (fd >= 0 && fd < 1024);
}

void	setup_builtin_redirs(t_mini *shell)
{
	if (is_valid_fd(shell->out) && shell->out != STDOUT_FILENO)
	{
		if (dup2(shell->out, STDOUT_FILENO) == -1)
			perror("minishell: dup2");
		close(shell->out);
		shell->out = STDOUT_FILENO;
	}
	if (is_valid_fd(shell->in) && shell->in != STDIN_FILENO)
	{
		if (dup2(shell->in, STDIN_FILENO) == -1)
			perror("minishell: dup2");
		close(shell->in);
		shell->in = STDIN_FILENO;
	}
}

void	restore_std_fds(int saved_stdout, int saved_stdin)
{
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("minishell: dup2");
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("minishell: dup2");
	close(saved_stdout);
	close(saved_stdin);
}

void	setup_child_signals_and_fds(t_mini *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (is_valid_fd(shell->out) && shell->out != STDOUT_FILENO)
	{
		dup2(shell->out, STDOUT_FILENO);
		close(shell->out);
	}
	if (is_valid_fd(shell->in) && shell->in != STDIN_FILENO)
	{
		dup2(shell->in, STDIN_FILENO);
		close(shell->in);
	}
}

void	handle_parent_wait(pid_t pid, t_mini *shell)
{
	int	status;

	g_sig.pid = pid;
	waitpid(pid, &status, 0);
	g_sig.pid = 0;
	if (WIFEXITED(status))
		shell->ret = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->ret = 128 + WTERMSIG(status);
}
