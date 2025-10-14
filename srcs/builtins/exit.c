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

static void	handle_numeric_arg(t_mini *mini, char *arg)
{
	long long int	exit_code;
	char			*endptr;

	errno = 0;
	exit_code = ft_strtoll(arg, &endptr, 10);
	if (errno == ERANGE || *endptr != '\0'
		|| exit_code < -9223372036854775807LL - 1
		|| exit_code > 9223372036854775807LL)
	{
		mini->ret = 2;
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(arg, STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
	}
	else
		mini->ret = (unsigned char)exit_code;
}

int	ft_exit(char **args, t_mini *mini)
{
	int	exit_code;

	if (args[1] && args[2])
	{
		mini->exit = 1;
		mini->ret = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		return (1);
	}
	if (args[1] && !is_valid_number(args[1]))
	{
		exit_code = 2;
		mini->ret = exit_code;
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(args[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
	}
	else if (args[1])
	{
		handle_numeric_arg(mini, args[1]);
		exit_code = mini->ret;
	}
	else
		exit_code = mini->ret;
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR);
	mini->exit = 1;
	return (exit_code);
}
