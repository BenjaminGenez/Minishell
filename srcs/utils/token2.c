/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:34:49 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:34:50 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	count_token_args(t_token *token)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = token;
	while (tmp && tmp->type != PIPE && tmp->type != END)
	{
		if (tmp->type == ARG || tmp->type == CMD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**token_to_args(t_token *token)
{
	char	**args;
	t_token	*tmp;
	int		i;
	int		count;

	count = count_token_args(token);
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	tmp = token;
	while (tmp && tmp->type != PIPE && tmp->type != END)
	{
		if (tmp->type == ARG || tmp->type == CMD)
			args[i++] = ft_strdup(tmp->str);
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}
