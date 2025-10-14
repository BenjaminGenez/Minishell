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

// History-related functions moved to srcs/utils/history.c
int	parse(t_mini *mini)
{
	char	*line;
	int		ret;
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	if (mini->ret)
		ft_putstr_fd("", STDERR);
	else
		ft_putstr_fd("", STDERR);
	ft_putstr_fd("\033[0;36m\033[1mminishell ▸ \033[0m", STDERR);
	line = read_line_with_history(mini);
	if (!line)  
	{
		ft_putendl_fd("exit", STDERR);
		mini->exit = 1;
		return (1);
	}
	if (ft_strlen(line) == 0 || (ft_strlen(line) == 1 && line[0] == '\n'))
	{
		free(line);
		return (0);
	}
	if (line[0] != '\0' && line[0] != '\n')
	{
		int len = ft_strlen(line);
		if (len > 0 && line[len-1] == '\n')
			line[len-1] = '\0';
		add_to_history(mini, line);
	}
	if (ft_strncmp(line, "exit", 4) == 0 && 
		(line[4] == '\0' || line[4] == ' ' || line[4] == '\n'))
	{
		free(line);
		mini->exit = 1;
		mini->ret = 0;
		return (1);
	}
	ret = process_line(mini, line, ft_strlen(line));
	free(line);
	return (ret);
}
