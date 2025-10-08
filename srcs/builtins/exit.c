/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/10/08 14:18:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || *str == '\0')
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_numeric_arg(t_mini *shell, char *arg)
{
	long long int	exit_code;
	char			*endptr;

	errno = 0;
	exit_code = ft_strtoll(arg, &endptr, 10);
	if (errno == ERANGE || *endptr != '\0'
		|| exit_code < -9223372036854775807LL - 1
		|| exit_code > 9223372036854775807LL)
	{
		shell->ret = 2;
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(arg, STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
	}
	else
		shell->ret = (unsigned char)exit_code;
}

void	mini_exit(t_mini *shell, char **cmd_args)
{
	int	exit_code;

	ft_putendl_fd("DEBUG: mini_exit called", STDERR);
	ft_putendl_fd("exit", STDOUT);
	if (cmd_args[1] && cmd_args[2])
	{
		shell->exit = 0;
		shell->ret = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		return ;
	}
	if (cmd_args[1] && !is_valid_number(cmd_args[1]))
	{
		exit_code = 2;
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(cmd_args[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
	}
	else if (cmd_args[1])
	{
		handle_numeric_arg(shell, cmd_args[1]);
		exit_code = shell->ret;
	}
	else
	{
		exit_code = 0;
	}
	cleanup_shell(shell);
	exit(exit_code);
}
