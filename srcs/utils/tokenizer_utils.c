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
	token.type = 0;

	// Check for operators first
	if (is_sep(line, *i) == 2) // Handle here-document operator <<
	{
		token.str = ft_strdup("<<");
		token.type = HEREDOC;
		(*i) += 2;
		return (token);
	}
	else if (is_sep(line, *i) == 1) // Handle other separators
	{
		token.str = ft_strndup(&line[*i], 1);
		token.type = line[*i];
		(*i)++;
		return (token);
	}

	// Handle regular tokens
	while (line && line[*i] && is_sep(line, *i) == 0)
	{
		if (line[*i] == '\\')
		{
			token.str = process_escaped_char(line, i, token.str);
		}
		else if (line[*i] == ' ' || line[*i] == '\t')
		{
			(*i)++;
			break;
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
	new_token->type = token.type; // Ensure the type is preserved
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

	printf("\n=== get_tokens called with: %s\n", line);
	i = 0;
	first = NULL;
	prev = NULL;

	// Skip leading whitespace
	while (line && (line[i] == ' ' || line[i] == '\t'))
		i++;

	while (line && line[i])
	{
		// Skip whitespace between tokens
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		
		if (!line[i])
			break;

		token = next_token(line, &i);
		if (token.str == NULL)
		{
			printf("No more tokens to process\n");
			break;
		}

		printf("Created token: '%s', type: %d\n", token.str, token.type);

		prev = create_new_token(token, prev, first);
		if (!prev)
		{
			printf("Error creating new token, cleaning up\n");
			free_tokens(first);
			return (NULL);
		}

		if (!first)
		{
			first = prev;
			printf("First token set to: '%s'\n", first->str);
		}

		// Skip whitespace after token
		while (line[i] == ' ' || line[i] == '\t')
			i++;
	}
	printf("=== Finished tokenizing input ===\n\n");

	return (first);
}
