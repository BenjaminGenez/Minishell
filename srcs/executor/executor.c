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

static void	execute_command(t_mini *shell, char **cmd_args, t_token *token)
{
	int	exit_code;
	int	saved_stdout;
	int	i;

	if (!cmd_args || !cmd_args[0])
	{
		ft_putendl_fd("DEBUG: execute_command: No command provided", STDERR);
		return ;
	}
	ft_putstr_fd("DEBUG: execute_command - cmd: ", STDERR);
	ft_putendl_fd(cmd_args[0], STDERR);
	i = 0;
	while (cmd_args[i])
	{
		ft_putstr_fd("DEBUG: execute_command: arg[", STDERR);
		ft_putnbr_fd(i, STDERR);
		ft_putstr_fd("]: ", STDERR);
		ft_putendl_fd(cmd_args[i], STDERR);
		i++;
	}
	if (ft_strcmp(cmd_args[0], "exit") == 0)
	{
		ft_putendl_fd("DEBUG: exit command detected", STDERR);
		if (has_pipe(token) == 0)
		{
			ft_putendl_fd("DEBUG: No pipe, calling mini_exit", STDERR);
			mini_exit(shell, cmd_args);
			ft_putendl_fd("DEBUG: Forcing exit after mini_exit", STDERR);
			exit(shell->ret);
		}
		else
		{
			ft_putendl_fd("DEBUG: Pipe detected, not exiting", STDERR);
		}
	}
	else if (is_builtin(cmd_args[0]))
	{
		ft_putendl_fd("DEBUG: Builtin command detected", STDERR);
		saved_stdout = dup(STDOUT);
		if (saved_stdout == -1)
		{
			perror("minishell: dup");
			shell->ret = 1;
			return ;
		}
		ft_putstr_fd("DEBUG: Saved stdout: ", STDERR);
		ft_putnbr_fd(saved_stdout, STDERR);
		ft_putchar_fd('\n', STDERR);
		if (shell->out != STDOUT)
		{
			ft_putstr_fd("DEBUG: Redirecting output to fd: ", STDERR);
			ft_putnbr_fd(shell->out, STDERR);
			ft_putchar_fd('\n', STDERR);
			if (dup2(shell->out, STDOUT) == -1)
			{
				perror("minishell: dup2");
				close(saved_stdout);
				shell->ret = 1;
				return ;
			}
			close(shell->out);
		}
		exit_code = exec_builtin(cmd_args, shell);
		shell->ret = exit_code;
		ft_putendl_fd("DEBUG: Restoring original stdout", STDERR);
		if (dup2(saved_stdout, STDOUT) == -1)
		{
			perror("minishell: dup2");
			shell->ret = 1;
		}
		close(saved_stdout);
	}
	else
	{
		ft_putendl_fd("DEBUG: External command detected", STDERR);
		shell->ret = exec_bin(cmd_args, shell->env, shell);
	}
	ft_putstr_fd("DEBUG: execute_command completed with exit code: ", STDERR);
	ft_putnbr_fd(shell->ret, STDERR);
	ft_putchar_fd('\n', STDERR);
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

	ft_putendl_fd("\nDEBUG: exec_cmd: Starting command execution", STDERR);

	if (shell->charge == 0)
	{
		ft_putendl_fd("DEBUG: exec_cmd: charge is 0, returning early", STDERR);
		return ;
	}

	ft_putendl_fd("DEBUG: exec_cmd: Building command array from token", STDERR);
	cmd_args = build_cmd_array(token);

	if (!cmd_args)
	{
		ft_putendl_fd("DEBUG: exec_cmd: cmd_args is NULL after build_cmd_array", STDERR);
		return ;
	}

	// Print the command arguments before expansion
	ft_putendl_fd("DEBUG: exec_cmd: Command arguments before expansion:", STDERR);
	for (int i = 0; cmd_args[i]; i++)
	{
		ft_putstr_fd("  - cmd_args[", STDERR);
		ft_putnbr_fd(i, STDERR);
		ft_putstr_fd("] = \"", STDERR);
		ft_putstr_fd(cmd_args[i], STDERR);
		ft_putstr_fd("\" (addr: ", STDERR);
		ft_putnbr_fd((long)cmd_args[i], 16);
		ft_putstr_fd(")\n", STDERR);
	}

	ft_putendl_fd("\nDEBUG: exec_cmd: Expanding command arguments", STDERR);
	expand_command_args(cmd_args, shell);

	// Print the command arguments after expansion
	ft_putendl_fd("DEBUG: exec_cmd: Command arguments after expansion:", STDERR);
	if (!cmd_args)
	{
		ft_putendl_fd("  - cmd_args is NULL after expansion", STDERR);
	}
	else
	{
		for (int i = 0; cmd_args[i]; i++)
		{
			ft_putstr_fd("  - cmd_args[", STDERR);
			ft_putnbr_fd(i, STDERR);
			ft_putstr_fd("] = \"", STDERR);
			ft_putstr_fd(cmd_args[i], STDERR);
			ft_putstr_fd("\" (addr: ", STDERR);
			ft_putnbr_fd((long)cmd_args[i], 16);
			ft_putstr_fd(")\n", STDERR);
		}
	}

	if (cmd_args && cmd_args[0])
	{
		ft_putstr_fd("\nDEBUG: exec_cmd: Executing command: \"", STDERR);
		ft_putstr_fd(cmd_args[0], STDERR);
		ft_putstr_fd("\" with ", STDERR);
		int arg_count = 0;
		while (cmd_args[arg_count])
			arg_count++;
		ft_putnbr_fd(arg_count - 1, STDERR);
		ft_putendl_fd(" arguments", STDERR);

		execute_command(shell, cmd_args, token);
	}
	else
	{
		ft_putendl_fd("\nDEBUG: exec_cmd: No command to execute (cmd_args is NULL or empty)", STDERR);
	}

	ft_putendl_fd("\nDEBUG: exec_cmd: Freeing command arguments", STDERR);
	free_tab(cmd_args);
	cleanup_shell_fds(shell);
	ft_putendl_fd("DEBUG: exec_cmd: Command execution completed\n", STDERR);
}
