/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:49:03 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:49:04 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_external_cmd(t_mini *shell, char **cmd_args)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals_and_fds(shell);
		exec_bin(cmd_args, shell);
	}
	else if (pid > 0)
		handle_parent_wait(pid, shell);
	else
		perror("minishell: fork");
}

static int	save_and_check_fds(int *saved_stdout, int *saved_stdin)
{
	*saved_stdout = dup(STDOUT_FILENO);
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdout == -1 || *saved_stdin == -1)
	{
		perror("minishell: dup");
		return (0);
	}
	return (1);
}

void	execute_command(t_mini *shell, char **cmd_args)
{
	int	saved_stdout;
	int	saved_stdin;

	if (!cmd_args || !cmd_args[0])
		return ;
	if (ft_strcmp(cmd_args[0], "exit") == 0)
		shell->ret = ft_exit(cmd_args, shell);
	else if (is_builtin(cmd_args[0]))
	{
		if (!save_and_check_fds(&saved_stdout, &saved_stdin))
		{
			shell->ret = 1;
			return ;
		}
		setup_builtin_redirs(shell);
		shell->ret = exec_builtin(cmd_args, shell);
		restore_std_fds(saved_stdout, saved_stdin);
	}
	else
		exec_external_cmd(shell, cmd_args);
}

void	expand_command_args(char **cmd_args, t_mini *shell)
{
	int		idx;
	char	*expanded;

	if (!cmd_args || !shell)
		return ;
	idx = 0;
	while (cmd_args[idx])
	{
		expanded = cmd_args[idx];
		if (ft_strchr(expanded, '$') || ft_strchr(expanded, '"')
			|| ft_strchr(expanded, '\''))
		{
			expanded = handle_expansions(expanded, shell->env, shell->ret);
			if (expanded != cmd_args[idx])
			{
				free(cmd_args[idx]);
				cmd_args[idx] = expanded;
			}
		}
		idx++;
	}
}

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
	free_array(cmd_args);
	reset_standard_fds(shell);
}
