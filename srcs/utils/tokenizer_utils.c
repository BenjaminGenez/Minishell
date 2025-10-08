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

	ft_putstr_fd("DEBUG: next_token: Starting at position ", STDERR);
	ft_putnbr_fd(*i, STDERR);
	ft_putstr_fd(", char='", STDERR);
	if (line && line[*i])
		ft_putchar_fd(line[*i], STDERR);
	else
		ft_putstr_fd("\\0", STDERR);
	ft_putstr_fd("'\n", STDERR);

	token.str = NULL;
	while (line && line[*i] && is_sep(line, *i) == 0)
	{
		if (line[*i] == '\\')
		{
			ft_putendl_fd("DEBUG: next_token: Found escape character \\", STDERR);
			token.str = process_escaped_char(line, i, token.str);
		}
		else if (line[*i] == ' ' || line[*i] == '\t')
		{
			ft_putendl_fd("DEBUG: next_token: Found space or tab, breaking", STDERR);
			(*i)++;
			break ;
		}
		else
		{
			ft_putstr_fd("DEBUG: next_token: Adding char '", STDERR);
			ft_putchar_fd(line[*i], STDERR);
			ft_putstr_fd("' to token\n", STDERR);
			token.str = ft_strjoin_char(token.str, line[(*i)++]);
		}
	}

	ft_putstr_fd("DEBUG: next_token: Returning token: \"", STDERR);
	if (token.str)
		ft_putstr_fd(token.str, STDERR);
	else
		ft_putstr_fd("(null)", STDERR);
	ft_putstr_fd("\"\n", STDERR);

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

	ft_putendl_fd("\nDEBUG: get_tokens: Starting tokenization", STDERR);
	ft_putstr_fd("DEBUG: get_tokens: Input line: \"", STDERR);
	if (line)
		ft_putstr_fd(line, STDERR);
	else
		ft_putstr_fd("(null)", STDERR);
	ft_putstr_fd("\"\n", STDERR);

	i = 0;
	token_count = 0;
	first = NULL;
	prev = NULL;

	while (line && line[i])
	{
		ft_putstr_fd("\nDEBUG: get_tokens: Processing token ", STDERR);
		ft_putnbr_fd(token_count + 1, STDERR);
		ft_putstr_fd(" at position ", STDERR);
		ft_putnbr_fd(i, STDERR);
		ft_putstr_fd(", char='", STDERR);
		ft_putchar_fd(line[i], STDERR);
		ft_putstr_fd("'\n", STDERR);

		token = next_token(line, &i);

		if (token.str == NULL)
		{
			ft_putendl_fd("DEBUG: get_tokens: Reached end of tokens", STDERR);
			break ;
		}

		token_count++;
		ft_putstr_fd("DEBUG: get_tokens: Created token ", STDERR);
		ft_putnbr_fd(token_count, STDERR);
		ft_putstr_fd(": \"", STDERR);
		ft_putstr_fd(token.str, STDERR);
		ft_putstr_fd("\"\n", STDERR);

		prev = create_new_token(token, prev, first);
		if (!prev)
		{
			ft_putendl_fd("DEBUG: get_tokens: Failed to create token node", STDERR);
			return (NULL);
		}

		if (!first)
		{
			first = prev;
			ft_putendl_fd("DEBUG: get_tokens: Set as first token", STDERR);
		}
	}

	ft_putstr_fd("DEBUG: get_tokens: Completed. Created ", STDERR);
	ft_putnbr_fd(token_count, STDERR);
	ft_putendl_fd(" tokens", STDERR);

	// Print the full token list
	if (first)
	{
		t_token *current = first;
		int idx = 0;
		ft_putendl_fd("DEBUG: get_tokens: Full token list:", STDERR);
		while (current)
		{
			ft_putstr_fd("  Token ", STDERR);
			ft_putnbr_fd(idx++, STDERR);
			ft_putstr_fd(": \"", STDERR);
			if (current->str)
				ft_putstr_fd(current->str, STDERR);
			else
				ft_putstr_fd("(null)", STDERR);
			ft_putstr_fd("\" (type: ", STDERR);
			ft_putnbr_fd(current->type, STDERR);
			ft_putstr_fd(")\n", STDERR);
			current = current->next;
		}
	}
	else
	{
		ft_putendl_fd("DEBUG: get_tokens: No tokens were created", STDERR);
	}

	return (first);
}
