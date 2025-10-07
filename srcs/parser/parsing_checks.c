/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:45:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:40:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_last_valid_arg(t_token *token)
{
	t_token	*prev;

	if (!token || is_type(token, CMD) || is_type(token, ARG))
	{
		prev = prev_sep(token, NOSKIP);
		if (!prev || is_type(prev, END) || is_type(prev, PIPE))
			return (1);
		return (0);
	}
	return (0);
}

int	validate_token(t_mini *mini, t_token *token)
{
	if (is_types(token, "TAI") && (!token->next
			|| is_types(token->next, "TAIPE")))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", STDERR);
		if (token->next)
			ft_putstr_fd(token->next->str, STDERR);
		else
			ft_putstr_fd("newline", STDERR);
		ft_putendl_fd("'", STDERR);
		mini->ret = 258;
		return (0);
	}
	if (is_types(token, "PE") && (!token->prev || !token->next
			|| is_types(token->prev, "TAIPE")))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", STDERR);
		ft_putstr_fd(token->str, STDERR);
		ft_putendl_fd("'", STDERR);
		mini->ret = 258;
		return (0);
	}
	return (1);
}

int	check_line(t_mini *mini, t_token *token)
{
	while (token)
	{
		if (!validate_token(mini, token))
			return (0);
		token = token->next;
	}
	return (1);
}
