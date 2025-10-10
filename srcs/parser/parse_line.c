/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/10 15:18:00 by user             ###   ########.fr       */
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

static void	handle_quotes(char c, int *single_quotes, int *double_quotes)
{
	if (c == '\'' && !(*double_quotes))
		*single_quotes = !(*single_quotes);
	else if (c == '"' && !(*single_quotes))
		*double_quotes = !(*double_quotes);
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
	mem_free(line);
	return (new);
}

int	quote_check(t_mini *mini, char **line)
{
	int	single_quotes;
	int	double_quotes;
	int	i;

	single_quotes = 0;
	double_quotes = 0;
	i = 0;
	while ((*line)[i])
	{
		handle_quotes((*line)[i], &single_quotes, &double_quotes);
		i++;
	}
	if (single_quotes || double_quotes)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR);
		mini->ret = 2;
		return (1);
	}
	return (0);
}
char	*clean_input(char *buffer, int bytes_read)
{
	int		i;
	char	*line;
	i = bytes_read - 1;
	while (i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r'))
		buffer[i--] = '\0';
	line = ft_strdup(buffer);
	return (line);
}
void	process_tokens(t_mini *mini)
{
	t_token	*token;
	token = mini->start;
	while (token)
	{
		type_arg(token, 0);
		token = token->next;
	}
}
int	process_line(t_mini *mini, char *buffer, int bytes_read)
{
	char	*line;
	line = clean_input(buffer, bytes_read);
	if (!line)
	{
		return (1);
	}
	if (g_sig.sigint == 1)
	{
		mini->ret = g_sig.exit_status;
	}
	if (quote_check(mini, &line))
	{
		mem_free(line);
		return (1);
	}
	line = space_line(line);
	if (line && line[0] == '$')
	{
		line[0] = (char)(-line[0]);
	}
	mini->start = get_tokens(line);
	mem_free(line);
	if (!mini->start)
	{
		return (1);
	}
	squish_args(mini);
	process_tokens(mini);
	return (0);
}
