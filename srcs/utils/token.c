/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:35:49 by user              #+#    #+#             */
/*   Updated: 2025/10/07 15:35:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/**
 * @brief Creates a new token with the given string and type
 * 
 * @param str The string for the token
 * @param type The type of the token
 * @return t_token* The new token, or NULL on failure
 */
t_token *create_token(char *str, int type)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->str = ft_strdup(str);
    if (!token->str)
    {
        free(token);
        return (NULL);
    }
    token->type = type;
    token->next = NULL;
    token->prev = NULL;
    return (token);
}

/**
 * @brief Adds a token to the end of the token list
 * 
 * @param tokens The head of the token list
 * @param new_token The new token to add
 */
void add_token(t_token **tokens, t_token *new_token)
{
    t_token *tmp;

    if (!tokens || !new_token)
        return;
    if (!*tokens)
    {
        *tokens = new_token;
        return;
    }
    tmp = *tokens;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_token;
    new_token->prev = tmp;
}

/**
 * @brief Frees all tokens in the list
 * 
 * @param tokens The head of the token list
 */
void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        if (tmp->str)
            free(tmp->str);
        free(tmp);
    }
}

void	type_arg(t_token *token, int separator)
{
	if (ft_strcmp(token->str, "") == 0) {
		token->type = EMPTY;
	} else if (ft_strcmp(token->str, ">") == 0 && separator == 0) {
		token->type = TRUNC;
	} else if (ft_strcmp(token->str, ">>") == 0 && separator == 0) {
		token->type = APPEND;
	} else if (ft_strcmp(token->str, "<") == 0 && separator == 0) {
		token->type = INPUT;
	} else if (ft_strcmp(token->str, "<<") == 0 && separator == 0) {
		token->type = HEREDOC;
	} else if (ft_strcmp(token->str, "|") == 0 && separator == 0) {
		token->type = PIPE;
	} else if (ft_strcmp(token->str, ";") == 0 && separator == 0) {
		token->type = END;
	} else if (token->prev == NULL || (token->prev && token->prev->type >= TRUNC)) {
		token->type = CMD;
	} else {
		token->type = ARG;
	}
}
void	move_token_to_prev(t_token *token, t_token *prev)
{
	token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	token->prev = prev;
	if (prev->next)
	{
		token->next = prev->next;
		token->next->prev = token;
	}
	else
		token->next = NULL;
	prev->next = token;
}
t_token	*next_sep(t_token *token, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type < TRUNC)
		token = token->next;
	return (token);
}
t_token	*prev_sep(t_token *token, int skip)
{
	if (token && skip)
		token = token->prev;
	while (token && token->type < TRUNC)
		token = token->prev;
	return (token);
}
t_token	*next_run(t_token *token, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type != CMD)
	{
		token = token->next;
		if (token && token->type == CMD && token->prev == NULL)
			;
		else if (token && token->type == CMD && token->prev->type < END)
			token = token->next;
	}
	return (token);
}
