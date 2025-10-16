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
