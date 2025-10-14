/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:16:56 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 19:30:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_history_ctx	*get_history_ctx(t_history_ctx *ctx)
{
	static t_history_ctx	static_ctx = {0};

	if (ctx)
		static_ctx = *ctx;
	return (&static_ctx);
}

void	init_terminal(t_history_ctx *ctx)
{
    struct termios	new_termios;

    if (!ctx)
        return ;
        
    tcgetattr(STDIN_FILENO, &ctx->original_termios);
    new_termios = ctx->original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    
    // Save the context
    get_history_ctx(ctx);
}

void	reset_terminal(t_history_ctx *ctx)
{
    if (!ctx)
        return ;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_termios);
}

void	add_to_history(t_mini *mini, char *line)
{
    (void)mini;
    if (!line || !*line)
        return ;
    add_history(line);
}

void	free_history(t_history *history)
{
    if (!history)
        return ;
    
    while (history->prev)
        history = history->prev;
        
    while (history)
    {
        t_history *next = history->next;
        if (history->command)
            free(history->command);
        free(history);
        history = next;
    }
}

char	*read_line_with_history(t_mini *mini)
{
	char	*line;

	(void)mini;
	line = readline("minishell> ");
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}
