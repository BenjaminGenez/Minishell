/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:46:20 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:50:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*
** This file contains the main executor functionality.
** It handles the execution of commands, both built-in and external.
*/
static int	is_valid_fd(int fd)
{
	return (fd >= 0 && fd < 1024);
}
static void	execute_command(t_mini *shell, char **cmd_args)
{
	int	saved_stdout;
	int	saved_stdin;
	if (!cmd_args || !cmd_args[0])
		return;
	if (ft_strcmp(cmd_args[0], "exit") == 0)
	{
		mini_exit(shell, cmd_args);
		return;
	}
	else if (is_builtin(cmd_args[0]))
	{
		saved_stdout = dup(STDOUT);
		if (saved_stdout == -1)
		{
			perror("minishell: dup");
			shell->ret = 1;
			return;
		}
		saved_stdin = dup(STDIN);
		if (saved_stdin == -1)
		{
			close(saved_stdout);
			perror("minishell: dup");
			shell->ret = 1;
			return;
		}
		if (is_valid_fd(shell->out) && shell->out != STDOUT)
		{
			if (dup2(shell->out, STDOUT) == -1)
			{
				perror("minishell: dup2");
				close(saved_stdout);
				close(saved_stdin);
				shell->ret = 1;
				return;
			}
			close(shell->out);
			shell->out = STDOUT;
		}
		if (is_valid_fd(shell->in) && shell->in != STDIN)
		{
			if (dup2(shell->in, STDIN) == -1)
			{
				perror("minishell: dup2");
				dup2(saved_stdout, STDOUT);
				close(saved_stdout);
				close(saved_stdin);
				shell->ret = 1;
				return;
			}
			close(shell->in);
			shell->in = STDIN;
		}
		shell->ret = exec_builtin(cmd_args, shell);
		if (dup2(saved_stdout, STDOUT) == -1 || dup2(saved_stdin, STDIN) == -1)
		{
			perror("minishell: dup2");
			shell->ret = 1;
		}
		close(saved_stdout);
		close(saved_stdin);
	}
	else
	{
		shell->ret = exec_bin(cmd_args, shell->env, shell);
	}
}
static void	expand_command_args(char **cmd_args, t_mini *shell)
{
	int		idx;
	char	*expanded;
	if (!cmd_args)
		return ;
	idx = 0;
	while (cmd_args[idx])
	{
		expanded = expansions(cmd_args[idx], shell->env, shell->ret);
		if (expanded != cmd_args[idx])
		{
			free(cmd_args[idx]);
			cmd_args[idx] = expanded;
		}
		idx++;
	}
}
void	exec_cmd(t_mini *shell, t_token *token)
{
	char	**cmd_args;
	if (shell->charge == 0)
	{
		return ;
	}
	cmd_args = build_cmd_array(token);
	if (!cmd_args)
	{
		return ;
	}
	expand_command_args(cmd_args, shell);
	if (cmd_args && cmd_args[0])
	{
		if (ft_strcmp(cmd_args[0], "cd") == 0)
			shell->ret = ft_cd(shell, cmd_args);
		else
			execute_command(shell, cmd_args);
	}
	free_tab(cmd_args);
	cleanup_shell_fds(shell);
}
