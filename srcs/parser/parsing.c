/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/14 14:15:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
}

char	*read_input_line(t_mini *mini)
{
	char	*line;

	if (mini->ret)
		ft_putstr_fd("", STDERR);
	else
		ft_putstr_fd("", STDERR);
	ft_putstr_fd("\033[0;36m\033[1mminishell ▸ \033[0m", STDERR);
	line = read_line_with_history(mini);
	return (line);
}

int	handle_empty_line(char *line)
{
	if (!line)
	{
		ft_putendl_fd("exit", STDERR);
		return (1);
	}
	if (ft_strlen(line) == 0 || (ft_strlen(line) == 1 && line[0] == '\n'))
	{
		free(line);
		return (1);
	}
	return (0);
}

void	add_line_to_history(t_mini *mini, char *line)
{
	int	len;

	if (line[0] != '\0' && line[0] != '\n')
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		add_to_history(mini, line);
	}
}

int	handle_exit_command(char *line, t_mini *mini)
{
	if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0'
			|| line[4] == ' ' || line[4] == '\n'))
	{
		free(line);
		mini->exit = 1;
		mini->ret = 0;
		return (1);
	}
	return (0);
}
