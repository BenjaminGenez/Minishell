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
