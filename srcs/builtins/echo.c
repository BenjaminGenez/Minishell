/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/10/09 12:31:21 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
static int	is_n_option(const char *arg)
{
	int	i;
	if (!arg || ft_strncmp(arg, "-n", 2) != 0)
		return (0);
	i = 2;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}
int	ft_echo(char **args)
{
	int	i;
	int	n_option;
	int	first_arg;
	i = 1;
	n_option = 0;
	while (args[i] && is_n_option(args[i]))
	{
		n_option = 1;
		i++;
	}
	first_arg = i;
	while (args[i])
	{
		write(STDOUT, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT, " ", 1);
		i++;
	}
	if (!n_option || first_arg == 1)
		write(STDOUT, "\n", 1);
	return (SUCCESS);
}
