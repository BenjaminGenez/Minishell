/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/13 20:45:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*space_alloc(char *line)
{
	char	*new;
	int		count;
	int		i;

	if (!line)
		return (NULL);
	count = 0;
	i = 0;
	while (line[i])
	{
		if (is_sep(line, i))
			count++;
		i++;
	}
	new = malloc(sizeof(char) * (i + 2 * count + 1));
	if (!new)
		return (NULL);
	return (new);
}

char	*space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	if (!line)
		return (NULL);
	new = space_alloc(line);
	if (!new)
	{
		free(line);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (line[i])
		handle_quotes_and_spaces(line, new, &i, &j);
	new[j] = '\0';
	free(line);
	return (new);
}

char	*clean_input(char *buffer, int bytes_read)
{
	int	i;

	if (!buffer || bytes_read <= 0)
		return (NULL);
	i = 0;
	while (i < bytes_read)
	{
		if (buffer[i] == '\\' && buffer[i + 1] == '\\')
			i += 2;
		else if (buffer[i] == '\\' && buffer[i + 1])
			i += 2;
		else
			i++;
	}
	buffer[bytes_read] = '\0';
	return (buffer);
}

static void	process_line_tokenize(t_mini *mini, char *line)
{
	if (line[0] == '$')
		line[0] = (char)(-line[0]);
	mini->start = get_tokens(line);
	mem_free(line);
}

static int	process_line_heredoc(t_mini *mini)
{
	squish_args(mini);
	process_tokens(mini);
	if (contains_heredoc(mini->start))
		process_tokens_heredoc(mini);
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
	if (g_sig.sigint == 1)
	{
		mini->ret = g_sig.exit_status;
		mem_free(line);
		return (1);
	}
	if (quote_check(mini, &line))
	{
		mem_free(line);
		return (1);
	}
	line = space_line(line);
	if (!line)
		return (1);
	process_line_tokenize(mini, line);
	if (!mini->start)
		return (1);
	return (process_line_heredoc(mini));
}
