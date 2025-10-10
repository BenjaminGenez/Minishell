/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 22:46:30 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:51:06 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
void	redir(t_mini *mini, t_token *token, int type)
{
	ft_close(mini->fdout);
	if (type == TRUNC)
		mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
	else
		mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	if (mini->fdout == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(token->str, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	dup2(mini->fdout, STDOUT);
}
void	input(t_mini *mini, t_token *token)
{
	ft_close(mini->fdin);
	mini->fdin = open(token->str, O_RDONLY, S_IRWXU);
	if (mini->fdin == -1)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(token->str, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	dup2(mini->fdin, STDIN);
}
int	minipipe(t_mini *mini)
{
	int	pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	mini->saved_stdout = dup(STDOUT);
	if (mini->saved_stdout == -1)
	{
		perror("minishell: dup");
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	if (dup2(pipefd[1], STDOUT) == -1)
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
