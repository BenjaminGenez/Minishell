/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:30:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 15:30:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	current = tokens;
	while (current != NULL)
	{
		next = current->next;
		if (current->str != NULL)
			free(current->str);
		free(current);
		current = next;
	}
}
static char	*process_escaped_char(char *line, int *i, char *token_str)
{
	if (line[*i + 1] && line[*i + 1] != ' ' && line[*i + 1] != '\t')
	{
		token_str = ft_strjoin_char(token_str, line[(*i)++]);
		token_str = ft_strjoin_char(token_str, line[(*i)++]);
	}
	return (token_str);
}
t_token	next_token(char *line, int *i)
{
	t_token	token;
	token.str = NULL;
	while (line && line[*i] && is_sep(line, *i) == 0)
	{
		if (line[*i] == '\\')
		{
			token.str = process_escaped_char(line, i, token.str);
		}
		else if (line[*i] == ' ' || line[*i] == '\t')
		{
			(*i)++;
			break ;
		}
		else
		{
			token.str = ft_strjoin_char(token.str, line[(*i)++]);
		}
	}
	return (token);
}
static t_token	*create_new_token(t_token token, t_token *prev, t_token *first)
{
	t_token	*new_token;
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		free_tokens(first);
		return (NULL);
	}
	*new_token = token;
	new_token->prev = prev;
	new_token->next = NULL;
	if (prev)
		prev->next = new_token;
	return (new_token);
}
t_token	*get_tokens(char *line)
{
	t_token	*first;
	t_token	*prev;
	t_token	token;
	int		i;
	int		token_count;
	t_token	*current;
	int		idx;
	i = 0;
	token_count = 0;
	first = NULL;
	prev = NULL;
	while (line && line[i])
	{
		token = next_token(line, &i);
		if (token.str == NULL)
		{
			break ;
		}
		token_count++;
		prev = create_new_token(token, prev, first);
		if (!prev)
		{
			return (NULL);
		}
		if (!first)
		{
			first = prev;
		}
	}
	if (first)
	{
		current = first;
		idx = 0;
		while (current)
		{
			current = current->next;
		}
	}
	return (first);
}
