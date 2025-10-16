/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:35:54 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:35:55 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_history(t_history *history)
{
	if (!history)
		return ;
	while (history->prev)
		history = history->prev;
	free_history_list(history);
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
