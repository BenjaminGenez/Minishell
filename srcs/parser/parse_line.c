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

	line = clean_input(buffer, bytes_read);
	if (!line)
		return (1);
	if (g_sig.sigint == 1)
		mini->ret = g_sig.exit_status;
	if (quote_check(mini, &line))
	{
		mem_free(line);
		return (1);
	}
	line = space_line(line);
	if (line && line[0] == '$')
		line[0] = (char)(-line[0]);
	mini->start = get_tokens(line);
	mem_free(line);
	squish_args(mini);
	process_tokens(mini);
	return (0);
}
