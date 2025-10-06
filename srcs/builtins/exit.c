/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/10/04 00:30:19 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_exit(t_mini *shell, char **cmd_args)
{
	shell->exit = 1;
	ft_putstr_fd("exit", STDERR);
	if (cmd_args[1])
		ft_putendl_fd("", STDERR);
	else
		ft_putendl_fd("", STDERR);
	if (cmd_args[1] && cmd_args[2])
	{
		shell->ret = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
	}
	else if (cmd_args[1] && ft_strisnum(cmd_args[1]) == 0)
	{
		shell->ret = 255;
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(cmd_args[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
	}
	else if (cmd_args[1])
		shell->ret = ft_atoi(cmd_args[1]);
	else
		shell->ret = 0;
}
