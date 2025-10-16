/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:39:44 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:39:46 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_signal_check(t_mini *mini, char *line)
{
	if (g_sig.sigint == 1)
	{
		mini->ret = g_sig.exit_status;
		mem_free(line);
		return (1);
	}
	return (0);
}

int	handle_quote_check(t_mini *mini, char **line)
{
	if (quote_check(mini, line))
	{
		mem_free(*line);
		return (1);
	}
	return (0);
}

int	process_line(t_mini *mini, char *buffer, int bytes_read)
{
	char	*line;

	if (!mini || !buffer || bytes_read <= 0)
		return (1);
	line = clean_input(buffer, bytes_read);
	if (!line)
		return (1);
	if (handle_signal_check(mini, line))
		return (1);
	if (handle_quote_check(mini, &line))
		return (1);
	line = space_line(line);
	if (!line)
		return (1);
	process_line_tokenize(mini, line);
	if (!mini->start)
		return (1);
	return (process_line_heredoc(mini));
}
