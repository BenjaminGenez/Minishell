/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:40:35 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:40:36 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error_token(t_token *token)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", STDERR);
	if (token)
		ft_putstr_fd(token->str, STDERR);
	else
		ft_putstr_fd("newline", STDERR);
	ft_putendl_fd("'", STDERR);
}

int	validate_redir_token(t_mini *mini, t_token *token)
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
	return (1);
}

int	validate_pipe_end_token(t_mini *mini, t_token *token)
{
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

int	validate_token(t_mini *mini, t_token *token)
{
	if (!token)
		return (0);
	if (!validate_redir_token(mini, token))
		return (0);
	if (!validate_pipe_end_token(mini, token))
		return (0);
	return (1);
}

int	check_line(t_mini *mini, t_token *token)
{
	if (!token)
		return (0);
	while (token)
	{
		if (!validate_token(mini, token))
			return (0);
		token = token->next;
	}
	return (1);
}
