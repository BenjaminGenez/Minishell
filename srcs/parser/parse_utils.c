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

void	handle_heredoc_sep(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = ' ';
	new[(*j)++] = line[(*i)++];
	new[(*j)++] = line[(*i)++];
	new[(*j)++] = ' ';
}

void	handle_other_sep(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = ' ';
	new[(*j)++] = line[(*i)++];
	if (line[*i] == '>')
		new[(*j)++] = line[(*i)++];
	new[(*j)++] = ' ';
}

void	handle_whitespace_seq(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = ' ';
	while (is_whitespace(line[*i]))
		(*i)++;
}

void	handle_escaped_char(char *line, char *new, int *i, int *j)
{
	new[(*j)++] = line[++(*i)];
	(*i)++;
}

void	handle_quoted_str(char *line, char *new, int *i, int *j)
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
