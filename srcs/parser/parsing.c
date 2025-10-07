/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/07 15:55:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_input(char *buffer, int *bytes_read)
{
	*bytes_read = read(0, buffer, BUFF_SIZE - 1);
	if (*bytes_read <= 0)
	{
		ft_putendl_fd("exit", STDERR);
		return (0);
	}
	buffer[*bytes_read] = '\0';
	return (1);
}

int	parse(t_mini *mini)
{
	char	buffer[BUFF_SIZE];
	int		bytes_read;

	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	if (mini->ret)
		ft_putstr_fd("", STDERR);
	else
		ft_putstr_fd("", STDERR);
	ft_putstr_fd("\033[0;36m\033[1mminishell ▸ \033[0m", STDERR);
	if (!read_input(buffer, &bytes_read))
	{
		mini->exit = 1;
		return (1);
	}
	return (process_line(mini, buffer, bytes_read));
}
