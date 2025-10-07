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
	if (ft_strcmp(cmd_args[0], "exit") == 0 && has_pipe(token) == 0)
		mini_exit(shell, cmd_args);
	else if (is_builtin(cmd_args[0]))
		shell->ret = exec_builtin(cmd_args, shell);
	else
		shell->ret = exec_bin(cmd_args, shell->env, shell);
}

static void	expand_command_args(char **cmd_args, t_mini *shell)
{
	int	idx;

	idx = 0;
	while (cmd_args && cmd_args[idx])
	{
		cmd_args[idx] = expansions(cmd_args[idx], shell->env, shell->ret);
		idx++;
	}
}

void	exec_cmd(t_mini *shell, t_token *token)
{
	char	**cmd_args;

	if (shell->charge == 0)
		return ;
	cmd_args = build_cmd_array(token);
	expand_command_args(cmd_args, shell);
	if (cmd_args && cmd_args[0])
		execute_command(shell, cmd_args, token);
	free_tab(cmd_args);
	cleanup_shell_fds(shell);
}
