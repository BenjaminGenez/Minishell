/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:46:07 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:46:09 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "env", "export", "unset",
		NULL};
	int			idx;

	if (!cmd_name)
		return (0);
	idx = 0;
	while (builtins[idx])
	{
		if (ft_strcmp(cmd_name, builtins[idx]) == 0)
			return (1);
		idx++;
	}
	return (0);
}

int	exec_builtin(char **cmd_args, t_mini *shell)
{
	int		exit_code;
	char	*cmd;
	int		i;

	if (!cmd_args || !cmd_args[0])
	{
		ft_putendl_fd("DEBUG: exec_builtin: No command provided", STDERR);
		return (1);
	}
	cmd = cmd_args[0];
	ft_putstr_fd("DEBUG: exec_builtin: Executing command: ", STDERR);
	ft_putendl_fd(cmd, STDERR);
	i = 0;
	while (cmd_args[i])
	{
		ft_putstr_fd("DEBUG: exec_builtin: arg[", STDERR);
		ft_putnbr_fd(i, STDERR);
		ft_putstr_fd("]: ", STDERR);
		ft_putendl_fd(cmd_args[i], STDERR);
		i++;
	}
	exit_code = 0;
	if (ft_strcmp(cmd, "echo") == 0)
	{
		ft_putendl_fd("DEBUG: exec_builtin: Calling ft_echo", STDERR);
		exit_code = ft_echo(cmd_args);
	}
	else if (ft_strcmp(cmd, "cd") == 0)
	{
		ft_putendl_fd("DEBUG: exec_builtin: Calling ft_cd", STDERR);
		exit_code = ft_cd(cmd_args, shell->env);
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
	{
		ft_putendl_fd("DEBUG: exec_builtin: Calling ft_pwd", STDERR);
		exit_code = ft_pwd();
	}
	else if (ft_strcmp(cmd, "env") == 0)
	{
		ft_putendl_fd("DEBUG: exec_builtin: Calling ft_env", STDERR);
		ft_env(shell->env);
		exit_code = 0;
	}
	else if (ft_strcmp(cmd, "export") == 0)
	{
		ft_putendl_fd("DEBUG: exec_builtin: Calling ft_export", STDERR);
		exit_code = ft_export(cmd_args, shell->env, shell->secret_env);
	}
	else if (ft_strcmp(cmd, "unset") == 0)
	{
		ft_putendl_fd("DEBUG: exec_builtin: Calling ft_unset", STDERR);
		exit_code = ft_unset(cmd_args, shell);
	}
	else
	{
		ft_putstr_fd("DEBUG: exec_builtin: Unknown command: ", STDERR);
		ft_putendl_fd(cmd, STDERR);
		exit_code = 1;
	}
	ft_putstr_fd("DEBUG: exec_builtin: Command completed with exit code: ", STDERR);
	ft_putnbr_fd(exit_code, STDERR);
	ft_putchar_fd('\n', STDERR);
	return (exit_code);
}
