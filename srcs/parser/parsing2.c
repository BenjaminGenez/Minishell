/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:41:09 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:41:10 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse(t_mini *mini)
{
	char	*line;
	int		ret;

	setup_signals();
	line = read_input_line(mini);
	if (handle_empty_line(line))
	{
		if (!line)
			mini->exit = 1;
		return (!line);
	}
	add_line_to_history(mini, line);
	if (handle_exit_command(line, mini))
		return (1);
	ret = process_line(mini, line, ft_strlen(line));
	free(line);
	return (ret);
}
