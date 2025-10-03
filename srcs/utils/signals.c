/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/10/03 23:41:57 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sig_int(int code)
{
    (void)code;
    if (g_sig.pid == 0)
    {
        ft_putstr_fd("\b\b  ", STDERR);
        ft_putstr_fd("\n", STDERR);
        ft_putstr_fd("\033[0;36m\033[1mminishell ▸ \033[0m", STDERR);
        g_sig.exit_status = 1;
    }
    else
    {
        ft_putstr_fd("\n", STDERR);
        g_sig.exit_status = 130;
    }
    g_sig.sigint = 1;
}

void sig_quit(int code)
{
    char *nbr;

    nbr = ft_itoa(code);
    if (g_sig.pid != 0)
    {
        ft_putstr_fd("Quit: ", STDERR);
        ft_putendl_fd(nbr, STDERR);
        g_sig.exit_status = 131;
        g_sig.sigquit = 1;
    }
    else
        ft_putstr_fd("\b\b  \b\b", STDERR);
    mem_free(nbr);
}

void sig_init(void)
{
    g_sig.sigint = 0;
    g_sig.sigquit = 0;
    g_sig.pid = 0;
    g_sig.exit_status = 0;
}