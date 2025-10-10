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

static struct termios g_orig_term;

static void	restore_terminal(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &g_orig_term);
}

void	setup_terminal(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return ;

	// Save original terminal settings
	if (tcgetattr(STDIN_FILENO, &g_orig_term) == -1)
	{
		perror("minishell: tcgetattr");
		exit(EXIT_FAILURE);
	}

	// Set up new terminal settings
	term = g_orig_term;
	term.c_lflag &= ~(ICANON);  // Disable canonical mode (line buffering)
	term.c_cc[VMIN] = 1;        // Minimum number of characters for noncanonical read
	term.c_cc[VTIME] = 0;       // Timeout in deciseconds for noncanonical read

	// Apply the new terminal settings
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("minishell: tcsetattr");
		exit(EXIT_FAILURE);
	}

	// Set up cleanup on exit
	atexit(restore_terminal);

	// Set line buffering for stdout
	setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
}
