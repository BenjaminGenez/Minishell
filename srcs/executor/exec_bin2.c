/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:19:41 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 14:20:08 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	return (1);
}

static int	setup_stdout_backup(t_mini *mini)
{
	mini->saved_stdout = dup(STDOUT_FILENO);
	if (mini->saved_stdout == -1)
	{
		perror("minishell: dup");
		return (0);
	}
	return (1);
}

static int	redirect_stdout_to_pipe(int pipefd[2], t_mini *mini)
{
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		close(pipefd[1]);
		close(mini->saved_stdout);
		return (0);
	}
	return (1);
}

int	minipipe(t_mini *mini)
{
	int	pipefd[2];

	if (!setup_pipe(pipefd))
		return (0);
	if (!setup_stdout_backup(mini))
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	if (!redirect_stdout_to_pipe(pipefd, mini))
		return (0);
	close(pipefd[1]);
	mini->pipin = pipefd[0];
	mini->last = 0;
	return (1);
}
