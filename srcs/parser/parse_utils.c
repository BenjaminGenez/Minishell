/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:54:54 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/14 13:54:54 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredoc_sep(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = ' ';
	new[(*j)++] = line[(*i)++];
	new[(*j)++] = line[(*i)++];
	new[(*j)++] = ' ';
}

static void	handle_other_sep(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = ' ';
	new[(*j)++] = line[(*i)++];
	if (line[*i] == '>')
		new[(*j)++] = line[(*i)++];
	new[(*j)++] = ' ';
}

static void	handle_whitespace_seq(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = ' ';
	while (is_whitespace(line[*i]))
		(*i)++;
}

static void	handle_quoted_str(char *line, char *new, int *i, int *j)
{
	char	quote;

	quote = line[(*i)++];
	new[(*j)++] = quote;
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '"' && line[(*i) + 1])
		{
			new[(*j)++] = line[(*i)++];
			new[(*j)++] = line[(*i)++];
		}
		else
			new[(*j)++] = line[(*i)++];
	}
	if (line[*i] == quote)
		new[(*j)++] = line[(*i)++];
}

void	handle_quotes_and_spaces(char *line, char *new, int *i, int *j)
{
	int	sep_type;

	if (!line || !new || !i || !j)
		return ;
	sep_type = is_sep(line, *i);
	if (sep_type == 2)
		handle_heredoc_sep(line, new, i, j);
	else if (sep_type == 1)
		handle_other_sep(line, new, i, j);
	else if (is_whitespace(line[*i]))
		handle_whitespace_seq(line, new, i, j);
	else if (line[*i] == '\\' && line[(*i) + 1])
	{
		new[(*j)++] = line[++(*i)];
		(*i)++;
	}
	else if (line[*i] == '"' || line[*i] == '\'')
		handle_quoted_str(line, new, i, j);
	else
		new[(*j)++] = line[(*i)++];
}
