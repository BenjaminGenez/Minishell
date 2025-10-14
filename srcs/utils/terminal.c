/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:15:00 by user              #+#    #+#             */
/*   Updated: 2025/10/10 16:23:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static struct	termios g_orig_term;

static void	restore_terminal(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_orig_term);
}

void	setup_terminal(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &g_orig_term) == -1)
	{
		perror("minishell: tcgetattr");
		exit(EXIT_FAILURE);
	}
	term = g_orig_term;
	term.c_lflag &= ~(ICANON);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("minishell: tcsetattr");
		exit(EXIT_FAILURE);
	}
	atexit(restore_terminal);
	setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
}
