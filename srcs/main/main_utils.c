/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:41:54 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:41:55 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_mini *shell, t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp && temp->type != PIPE && temp->type != END)
	{
		if (temp->type == INPUT)
			input_redirection(shell, temp->next);
		else if (temp->type == TRUNC || temp->type == APPEND)
			redir(shell, temp->next, temp->type);
		temp = temp->next;
	}
}

void	restore_stdout(t_mini *shell, int stdout_copy)
{
	if (shell->saved_stdout != -1)
	{
		dup2(shell->saved_stdout, STDOUT);
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
	dup2(stdout_copy, STDOUT);
	close(stdout_copy);
}

void	handle_redir_exec(t_mini *shell, t_token *token)
{
	int		stdout_copy;

	if (token->type != CMD)
		return ;
	if (shell->saved_stdout == -1)
		shell->saved_stdout = dup(STDOUT);
	stdout_copy = dup(STDOUT);
	if (stdout_copy == -1)
	{
		perror("minishell: dup");
		return ;
	}
	apply_redirections(shell, token);
	if (shell->no_exec == 0 && token->str[0] != '\0')
		exec_cmd(shell, token);
	restore_stdout(shell, stdout_copy);
}

void	handle_child_process(t_mini *shell)
{
	if (shell->pipin != STDIN)
	{
		dup2(shell->pipin, STDIN);
		close(shell->pipin);
		shell->pipin = STDIN;
	}
	if (shell->saved_stdout != -1)
	{
		dup2(shell->saved_stdout, STDOUT);
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
}

void	cleanup_tokens_env(t_mini *shell)
{
	if (shell->start)
	{
		free_tokens(shell->start);
		shell->start = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->secret_env)
	{
		free_env(shell->secret_env);
		shell->secret_env = NULL;
	}
}
