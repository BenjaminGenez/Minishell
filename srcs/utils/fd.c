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

void ft_close(int file_descriptor)
{
    if (file_descriptor > 0)
        close(file_descriptor);
}

void reset_std(t_mini *shell)
{
    dup2(shell->in, STDIN);
    dup2(shell->out, STDOUT);
}

void close_fds(t_mini *shell)
{
    ft_close(shell->fdin);
    ft_close(shell->fdout);
    ft_close(shell->pipin);
    ft_close(shell->pipout);
}

void reset_fds(t_mini *shell)
{
    shell->fdin = -1;
    shell->fdout = -1;
    shell->pipin = -1;
    shell->pipout = -1;
    shell->pid = -1;
}