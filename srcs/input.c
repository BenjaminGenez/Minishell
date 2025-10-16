/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:04:43 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/14 14:04:44 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**command_completion(const char *text, int start, int end)
{
	(void)end;
	if (start == 0 || rl_line_buffer[start - 1] == ' ')
		return (rl_completion_matches(text, rl_filename_completion_function));
	return (NULL);
}

void	setup_readline(void)
{
	g_signal.sigint = 0;
	rl_catch_signals = 0;
	rl_clear_signals();
	rl_clear_pending_input();
	rl_attempted_completion_function = command_completion;
}

char	*read_input(void)
{
	char	*input;
	char	*prompt;

	setup_readline();
	prompt = "minishell> ";
	if (isatty(STDOUT_FILENO) || isatty(STDERR_FILENO))
	{
		write(STDOUT_FILENO, "", 0);
		write(STDERR_FILENO, "", 0);
	}
	input = readline(prompt);
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}
