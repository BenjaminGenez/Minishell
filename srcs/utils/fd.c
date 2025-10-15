/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:48:23 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:48:24 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(int file_descriptor)
{
	if (file_descriptor >= 0 && file_descriptor < 1024)  
	{
		close(file_descriptor);
	}
}
void	reset_std(t_mini *shell)
{
	if (!shell)
		return ;
	if (shell->in >= 0 && shell->in != STDIN)
	{
		if (dup2(shell->in, STDIN) == -1)
			perror("minishell: dup2");
	}
	if (shell->out >= 0 && shell->out != STDOUT)
	{
		if (dup2(shell->out, STDOUT) == -1)
			perror("minishell: dup2");
	}
}
void	close_fds(t_mini *shell)
{
	ft_close(shell->fdin);
	ft_close(shell->fdout);
	ft_close(shell->pipin);
	ft_close(shell->pipout);
}
void	reset_fds(t_mini *shell)
{
	shell->fdin = -1;
	shell->fdout = -1;
	shell->pipin = -1;
	shell->pipout = -1;
	shell->pid = -1;
}
