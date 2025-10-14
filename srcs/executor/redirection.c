/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Forward declarations */


void	redir(t_mini *mini, t_token *token, int type)
{
	if (type == HEREDOC)
	{
		if (mini->fdin != STDIN_FILENO)
		{
			dup2(mini->fdin, STDIN_FILENO);
			close(mini->fdin);
			mini->fdin = STDIN_FILENO;
		}
	}
	else if (type == TRUNC || type == APPEND)
	{
		if (mini->fdout != STDOUT_FILENO)
		{
			close(mini->fdout);
			mini->fdout = STDOUT_FILENO;
		}
		if (type == TRUNC)
			mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else
			mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (mini->fdout == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token->str, STDERR_FILENO);
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			mini->ret = 1;
			mini->no_exec = 1;
			return ;
		}
		dup2(mini->fdout, STDOUT_FILENO);
	}
	else if (type == INPUT)
	{
		input_redirection(mini, token);
	}
}

void	input_redirection(t_mini *mini, t_token *token)
{
	if (mini->fdin != STDIN_FILENO)
	{
		close(mini->fdin);
		mini->fdin = STDIN_FILENO;
	}
	mini->fdin = open(token->str, O_RDONLY);
	if (mini->fdin == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(token->str, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	dup2(mini->fdin, STDIN_FILENO);
}

int	minipipe(t_mini *mini)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	mini->saved_stdout = dup(STDOUT_FILENO);
	if (mini->saved_stdout == -1)
	{
		perror("minishell: dup");
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		close(pipefd[1]);
		close(mini->saved_stdout);
		return (0);
	}
	close(pipefd[1]);
	mini->pipin = pipefd[0];
	mini->last = 0;
	return (1);
}
