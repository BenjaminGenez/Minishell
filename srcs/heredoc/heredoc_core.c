/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:44:17 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:44:18 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_lines(const char *delimiter, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited "
				"by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd((char *)delimiter, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(pipefd[1], line, ft_strlen(line)) == -1
			|| write(pipefd[1], "\n", 1) == -1)
		{
			free(line);
			close(pipefd[1]);
			return (1);
		}
		free(line);
	}
	close(pipefd[1]);
	return (0);
}

static int	process_single_heredoc(t_mini *mini, t_token *token)
{
	int		pipefd[2];

	if (!token->next || !token->next->str)
		return (1);
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	signal(SIGINT, SIG_DFL);
	if (read_heredoc_lines(token->next->str, pipefd) != 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (mini->fdin != STDIN_FILENO)
		close(mini->fdin);
	mini->fdin = pipefd[0];
	close(pipefd[1]);
	signal(SIGINT, SIG_DFL);
	return (0);
}

int	handle_heredoc(t_mini *mini, t_token *token)
{
	return (process_single_heredoc(mini, token));
}

void	process_tokens_heredoc(t_mini *mini)
{
	t_token	*tmp;

	tmp = mini->start;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (process_single_heredoc(mini, tmp) != 0)
				break ;
		}
		tmp = tmp->next;
	}
}

int	read_heredoc_content(const char *delimiter, int pipefd[2])
{
	return (read_heredoc_lines(delimiter, pipefd));
}
