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

/*
** This file contains the main tokenizer functionality.
** The tokenizer is responsible for converting the input string into tokens
** that can be processed by the parser.
*/

/**
 * @brief Creates a list of tokens from the input line
 * 
 * @param line The input line to tokenize
 * @return t_token* The head of the token list, or NULL on error
 */
t_token *get_tokens(char *line)
{
    t_token *tokens;
    char    **args;
    int     i;

    if (!line || !*line)
        return (NULL);
    
    args = ft_split(line, ' '); // TODO: Implement proper quote handling in split
    if (!args)
        return (NULL);
    
    tokens = NULL;
    i = 0;
    while (args[i])
    {
        t_token *new_token = create_token(args[i], 0);
        if (!new_token)
        {
            free_tokens(tokens);
            ft_free_array(args);
            return (NULL);
        }
        add_token(&tokens, new_token);
        i++;
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

static int	is_escaped_char(char c1, char c2)
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

static int	process_escaped_sequence(char *line, int *i, int *count)
{
	if (is_escaped_char(line[*i + *count], line[*i + *count + 1]))
	{
		*count += 2;
		return (1);
	}
	return (0);
}

int	next_alloc(char *line, int *i)
{
	int	count;

	count = 0;
	while (line[*i + count] && is_sep(line, *i + count) == 0)
	{
		if (!process_escaped_sequence(line, i, &count))
			count++;
	}
	return (count);
}
