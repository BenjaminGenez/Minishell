/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/10/08 14:43:30 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	n_option;
	int	has_args;
	int	j;
	int	k;

	ft_putstr_fd("DEBUG: ft_echo: Starting with args: ", STDERR);
	if (!args)
	{
		ft_putendl_fd("(null)", STDERR);
	}
	else
	{
		k = 0;
		while (args[k])
		{
			ft_putstr_fd("[", STDERR);
			if (args[k])
				ft_putstr_fd(args[k], STDERR);
			else
				ft_putstr_fd("(null)", STDERR);
			ft_putstr_fd("] ", STDERR);
			k++;
		}
		ft_putchar_fd('\n', STDERR);
		ft_putendl_fd("DEBUG: ft_echo: Argument addresses:", STDERR);
		k = 0;
		while (args[k])
		{
			ft_putstr_fd("  - args[", STDERR);
			ft_putnbr_fd(k, STDERR);
			ft_putstr_fd("] = ", STDERR);
			ft_putnbr_fd((long)args[k], 16);
			ft_putstr_fd(" (\"", STDERR);
			ft_putstr_fd(args[k], STDERR);
			ft_putstr_fd("\")\n", STDERR);
			k++;
		}
	}
	if (!args || !args[0] || !args[1])
	{
		ft_putendl_fd("DEBUG: ft_echo: No arguments, printing newline", STDERR);
		write(STDERR, "DEBUG: ft_echo: Writing newline to STDOUT\n", 42);
		write(STDOUT, "\n", 1);
		return (SUCCESS);
	}

	i = 1;
	n_option = 0;
	ft_putendl_fd("DEBUG: ft_echo: Checking for -n option", STDERR);
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		ft_putstr_fd("DEBUG: ft_echo: Found -n option in arg ", STDERR);
		ft_putendl_fd(args[i], STDERR);

		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			n_option = 1;
			i++;
		}
		else
		{
			ft_putendl_fd("DEBUG: ft_echo: Not a valid -n option, breaking", STDERR);
			break ;
		}
	}

	has_args = (args[i] != NULL);
	ft_putstr_fd("DEBUG: ft_echo: has_args = ", STDERR);
	ft_putnbr_fd(has_args, STDERR);
	ft_putstr_fd(", n_option = ", STDERR);
	ft_putnbr_fd(n_option, STDERR);
	ft_putstr_fd(", i = ", STDERR);
	ft_putnbr_fd(i, STDERR);
	ft_putchar_fd('\n', STDERR);
	ft_putendl_fd("DEBUG: ft_echo: Printing arguments:", STDERR);
	while (args[i])
	{
		ft_putstr_fd("  - Printing: \"", STDERR);
		ft_putstr_fd(args[i], STDERR);
		ft_putstr_fd("\" (length: ", STDERR);
		ft_putnbr_fd(ft_strlen(args[i]), STDERR);
		ft_putstr_fd(")\n", STDERR);
		ssize_t written = write(STDOUT, args[i], ft_strlen(args[i]));
		if (written == -1)
		{
			perror("DEBUG: ft_echo: write failed");
		}
		if (args[i + 1])
		{
			write(STDERR, "DEBUG: ft_echo: Adding space between arguments\n", 46);
			write(STDOUT, " ", 1);
		}
		i++;
	}
	if (!n_option)
	{
		ft_putendl_fd("DEBUG: ft_echo: Adding newline", STDERR);
		write(STDERR, "DEBUG: ft_echo: Writing newline to STDOUT\n", 43);
		write(STDOUT, "\n", 1);
	}
	else
	{
		ft_putendl_fd("DEBUG: ft_echo: No newline (-n option)", STDERR);
	}

	ft_putendl_fd("DEBUG: ft_echo: Finished successfully", STDERR);
	return (SUCCESS);
}
