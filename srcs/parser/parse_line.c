/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/07 18:17:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*clean_input(char *buffer, int bytes_read)
{
	int		i;
	char	*line;

	i = bytes_read - 1;
	while (i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r'))
		buffer[i--] = '\0';
	line = ft_strdup(buffer);
	return (line);
}

void	process_tokens(t_mini *mini)
{
	t_token	*token;

	token = mini->start;
	while (token)
	{
		if (is_type(token, ARG))
			type_arg(token, 0);
		token = token->next;
	}
}

int	process_line(t_mini *mini, char *buffer, int bytes_read)
{
	char	*line;

	ft_putendl_fd("DEBUG: process_line started", STDERR);
	line = clean_input(buffer, bytes_read);
	if (!line)
	{
		ft_putendl_fd("DEBUG: clean_input returned NULL", STDERR);
		return (1);
	}
	ft_putstr_fd("DEBUG: Cleaned input: ", STDERR);
	ft_putendl_fd(line, STDERR);
	if (g_sig.sigint == 1)
	{
		ft_putendl_fd("DEBUG: SIGINT detected", STDERR);
		mini->ret = g_sig.exit_status;
	}
	if (quote_check(mini, &line))
	{
		ft_putendl_fd("DEBUG: quote_check failed", STDERR);
		mem_free(line);
		return (1);
	}
	line = space_line(line);
	ft_putstr_fd("DEBUG: After space_line: ", STDERR);
	ft_putendl_fd(line, STDERR);
	if (line && line[0] == '$')
	{
		ft_putendl_fd("DEBUG: Found $ at start of line", STDERR);
		line[0] = (char)(-line[0]);
	}
	ft_putendl_fd("DEBUG: Getting tokens...", STDERR);
	mini->start = get_tokens(line);
	mem_free(line);
	if (!mini->start)
	{
		ft_putendl_fd("DEBUG: No tokens generated", STDERR);
		return (1);
	}
	squish_args(mini);
	process_tokens(mini);
	ft_putendl_fd("DEBUG: process_line completed successfully", STDERR);
	return (0);
}
