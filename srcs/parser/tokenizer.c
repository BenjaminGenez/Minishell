/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:48:01 by user              #+#    #+#             */
/*   Updated: 2025/10/10 15:20:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_tokens_from_args(t_token **tokens, char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		add_token(tokens, create_token(args[i], 0));
		if (!*tokens)
			return (0);
		i++;
	}
	return (1);
}

t_token	*get_tokens(char *line)
{
	t_token	*tokens;
	char	**args;

	if (!line || !*line)
		return (NULL);
	args = ft_split(line, ' ');
	if (!args)
		return (NULL);
	tokens = NULL;
	if (!add_tokens_from_args(&tokens, args))
	{
		free_tokens(tokens);
		ft_free_array(args);
		return (NULL);
	}
	free_array(args);
	return (tokens);
}

void	squish_args(t_mini *mini)
{
	t_token	*token;
	t_token	*prev;
	t_token	*next;

	token = mini->start;
	while (token)
	{
		prev = prev_sep(token, NOSKIP);
		next = token->next;
		if (is_type(token, ARG) && is_types(prev, "TAI"))
		{
			while (is_last_valid_arg(prev) == 0)
				prev = prev->prev;
			move_token_to_prev(token, prev);
		}
		token = next;
	}
}

int	is_escaped_char(char c1, char c2)
{
	const char	escaped_chars[] = ";|<>\\ \t\n\r\v\f";
	int			i;

	i = 0;
	while (escaped_chars[i])
	{
		if (c1 == '\\' && c2 == escaped_chars[i])
			return (1);
		i++;
	}
	return (0);
}

int	process_escaped_sequence(char *line, int *i, int *count)
{
	if (is_escaped_char(line[*i + *count], line[*i + *count + 1]))
	{
		*count += 2;
		return (1);
	}
	return (0);
}


