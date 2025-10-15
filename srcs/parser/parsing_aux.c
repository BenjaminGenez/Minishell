/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:36:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:36:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_escaped_quote(char **line, int *i)
{
	if ((*line)[*i] == '\\' && ((*line)[*i + 1] == '\\' ||
		(*line)[*i + 1] == '"'))
	{
		*i += 2;
		return (1);
	}
	else if ((*line)[*i] == '\\' && (*line)[*i + 1])
	{
		*i += 2;
		return (1);
	}
	return (0);
}

static int	handle_quote_pair(char **line, int *i, t_mini *mini)
{
	char	quote;

	quote = (*line)[(*i)++];
	while ((*line)[*i] && (*line)[*i] != quote)
		(*i)++;
	if (!(*line)[*i])
	{
		ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
		ft_putstr_fd("unexpected end of file\n", STDERR_FILENO);
		mini->ret = 2;
		return (1);
	}
	(*i)++;
	return (0);
}

int	quote_check(t_mini *mini, char **line)
{
	int		i;

	if (!mini || !line || !*line)
		return (1);
	i = 0;
	while ((*line)[i])
	{
		if (handle_escaped_quote(line, &i))
			continue ;
		else if ((*line)[i] == '"' || (*line)[i] == '\'')
		{
			if (handle_quote_pair(line, &i, mini))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

void	process_tokens(t_mini *mini)
{
	t_token	*token;

	if (!mini || !mini->start)
		return ;
	token = mini->start;
	while (token)
	{
		type_arg(token, 0);
		token = token->next;
	}
}
