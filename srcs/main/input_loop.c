/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:02:15 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/15 00:00:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(t_mini *shell, char *input)
{
	if (!input || !*input)
		return ;
	process_input(shell, input);
	if (shell->start && shell->start->type != END)
	{
		if (has_pipe(shell->start))
			exec_pipeline(shell);
		else
			exec_cmd(shell, shell->start);
	}
	if (shell->start)
	{
		free_tokens(shell->start);
		shell->start = NULL;
	}
}

static void	process_readline_input(t_mini *shell, char *input)
{
	if (*input)
	{
		add_history(input);
		handle_input(shell, input);
	}
	else
		free(input);
}

void	input_loop(struct s_mini *shell)
{
	char	*input;
	char	*prompt;

	signal(SIGINT, sig_int);
	signal(SIGQUIT, SIG_IGN);
	if (isatty(STDIN_FILENO))
		prompt = "minishell> ";
	else
		prompt = "";
	while (!shell->exit)
	{
		g_sig.pid = 0;
		input = readline(prompt);
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		process_readline_input(shell, input);
	}
}
