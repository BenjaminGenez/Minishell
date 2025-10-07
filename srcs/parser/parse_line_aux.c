/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_aux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:35:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:35:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*space_alloc(char *line)
{
	char	*new;
	int		count;
	int		i;

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

static void	handle_quotes_and_spaces(char *line, char *new, int *i, int *j)
{
	if (is_sep(line, *i))
	{
		new[(*j)++] = ' ';
		new[(*j)++] = line[(*i)++];
		if (line[*i] == '>')
			new[(*j)++] = line[(*i)++];
		new[(*j)++] = ' ';
	}
	else if (line[*i] == '"' || line[*i] == '\'')
	{
		new[(*j)++] = line[(*i)++];
		while (line[*i] && ((line[*i - 1] == '\\' && line[*i] == '\"')
				|| (line[*i] != '"' && line[*i] != '\'')))
			new[(*j)++] = line[(*i)++];
		if (line[*i])
			new[(*j)++] = line[(*i)++];
	}
	else
		new[(*j)++] = line[(*i)++];
}

char	*space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = space_alloc(line);
	if (!new)
		return (NULL);
	while (line[i])
		handle_quotes_and_spaces(line, new, &i, &j);
	new[j] = '\0';
	return (new);
}

int	quote_check(t_mini *mini, char **line)
{
	char	*new_line;

	new_line = space_line(*line);
	mem_free(*line);
	*line = new_line;
	if (!new_line)
	{
		mini->ret = -1;
		return (1);
	}
	return (0);
}
