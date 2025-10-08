/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:32:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:32:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redir_exec(t_mini *shell, t_token *token)
{
	t_token	*prev_sep_token;
	t_token	*next_sep_token;
	int		pipe_status;

	prev_sep_token = prev_sep(token, NOSKIP);
	next_sep_token = next_sep(token, NOSKIP);
	pipe_status = 0;
	if (is_type(prev_sep_token, TRUNC))
		redir(shell, token, TRUNC);
	else if (is_type(prev_sep_token, APPEND))
		redir(shell, token, APPEND);
	else if (is_type(prev_sep_token, INPUT))
		input(shell, token);
	else if (is_type(prev_sep_token, PIPE))
		pipe_status = minipipe(shell);
	if (next_sep_token && is_type(next_sep_token, END) == 0 && pipe_status != 1)
		handle_redir_exec(shell, next_sep_token->next);
	if ((is_type(prev_sep_token, END) || is_type(prev_sep_token, PIPE)
			|| !prev_sep_token) && pipe_status != 1 && shell->no_exec == 0)
		exec_cmd(shell, token);
}

void	handle_child_process(t_mini *shell, int *child_status)
{
	waitpid(-1, child_status, 0);
	*child_status = WEXITSTATUS(*child_status);
	if (shell->last == 0)
		shell->ret = *child_status;
	if (shell->parent == 0)
	{
		free_token(shell->start);
		exit(shell->ret);
	}
}

void	exec_pipeline(t_mini *shell)
{
	t_token	*cmd;
	int		child_status;

	ft_putendl_fd("\nDEBUG: exec_pipeline: Starting pipeline execution", STDERR);

	// Get the first command to execute
	cmd = next_run(shell->start, NOSKIP);
	ft_putstr_fd("DEBUG: exec_pipeline: First command token: ", STDERR);
	if (cmd && cmd->str)
		ft_putendl_fd(cmd->str, STDERR);
	else
		ft_putendl_fd("(null)", STDERR);

	// Skip redirections at the start
	if (is_types(shell->start, "TAI"))
	{
		ft_putendl_fd("DEBUG: exec_pipeline: Found redirection at start, moving to next token", STDERR);
		cmd = shell->start->next;
	}

	// Process each command in the pipeline
	while (shell->exit == 0 && cmd)
	{
		ft_putstr_fd("\nDEBUG: exec_pipeline: Processing command: ", STDERR);
		if (cmd->str)
			ft_putendl_fd(cmd->str, STDERR);
		else
			ft_putendl_fd("(null)", STDERR);

		shell->charge = 1;
		shell->parent = 1;
		shell->last = 1;

		ft_putendl_fd("DEBUG: exec_pipeline: Calling handle_redir_exec", STDERR);
		handle_redir_exec(shell, cmd);

		ft_putendl_fd("DEBUG: exec_pipeline: Resetting std fds", STDERR);
		reset_std(shell);

		ft_putendl_fd("DEBUG: exec_pipeline: Closing file descriptors", STDERR);
		close_fds(shell);

		ft_putendl_fd("DEBUG: exec_pipeline: Resetting file descriptors", STDERR);
		reset_fds(shell);

		ft_putendl_fd("DEBUG: exec_pipeline: Handling child process", STDERR);
		handle_child_process(shell, &child_status);

		shell->no_exec = 0;

		ft_putendl_fd("DEBUG: exec_pipeline: Getting next command in pipeline", STDERR);
		cmd = next_run(cmd, SKIP);
		if (cmd)
		{
			ft_putstr_fd("DEBUG: exec_pipeline: Next command: ", STDERR);
			if (cmd->str)
				ft_putendl_fd(cmd->str, STDERR);
			else
				ft_putendl_fd("(null)", STDERR);
		}
		else
		{
			ft_putendl_fd("DEBUG: exec_pipeline: No more commands in pipeline", STDERR);
		}
	}

	ft_putendl_fd("DEBUG: exec_pipeline: Pipeline execution completed\n", STDERR);
}
