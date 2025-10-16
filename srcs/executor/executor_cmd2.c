/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:03:20 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 14:04:02 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_redirections(t_mini *shell, t_token *token)
{
	while (token && token->type != END && token->type != PIPE)
	{
		if (token->type == TRUNC || token->type == APPEND
			|| token->type == INPUT || token->type == HEREDOC)
		{
			handle_redirection(shell, token, token->type);
			if (shell->no_exec == 1)
				return (0);
			token = token->next;
		}
		token = token->next;
	}
	return (1);
}

void	exec_cmd(t_mini *shell, t_token *token)
{
	char	**cmd_args;
	t_token	*start_token;

	if (!token || !shell)
		return ;
	start_token = token;
	if (!process_redirections(shell, token))
	{
		reset_standard_fds(shell);
		return ;
	}
	cmd_args = token_list_to_array(start_token);
	if (!cmd_args)
	{
		reset_standard_fds(shell);
		return ;
	}
	expand_command_args(cmd_args, shell);
	if (cmd_args && cmd_args[0])
		execute_command(shell, cmd_args);
	ft_free_array(cmd_args);
	reset_standard_fds(shell);
}
