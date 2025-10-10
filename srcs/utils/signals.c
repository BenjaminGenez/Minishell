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
void	sig_int(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		g_sig.exit_status = 1;
		if (isatty(STDIN_FILENO))
		{
			write(STDERR_FILENO, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
	else
	{
		write(STDERR_FILENO, "^C\n", 3);
		kill(-g_sig.pid, SIGINT);
		g_sig.exit_status = 130;
	}
}
void	sig_quit(int code)
{
	(void)code;
	if (g_sig.pid != 0)
	{
		write(STDERR_FILENO, "^\
", 3);
		kill(-g_sig.pid, SIGQUIT);
		g_sig.exit_status = 131;
	}
}
void	sig_init(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}
