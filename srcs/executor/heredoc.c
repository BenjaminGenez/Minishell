/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:30:00 by user              #+#    #+#             */
/*   Updated: 2025/10/13 18:30:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_name(void)
{
	static int	i;
	char		*name;
	char		*number;

	i = 0;
	number = ft_itoa(i++);
	name = ft_strjoin("/tmp/heredoc_", number);
	free(number);
	return (name);
}

static int	create_heredoc(t_redir *redir, char *delimiter)
{
	int		fd;
	char	*line;
	char	*name;

	name = get_heredoc_name();
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		free(name);
		return (0);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	redir->file = name;
	return (1);
}

int	handle_heredoc(t_redir *redir, t_mini *mini)
{
	t_token	*token;

	token = redir->token;
	if (!token->next || token->next->type != ARG)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
			STDERR);
		mini->ret = 258;
		return (0);
	}
	if (!create_heredoc(redir, token->next->str))
		return (0);
	return (1);
}
