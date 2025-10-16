/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:44:17 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 14:32:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_mini_fdin(t_mini *mini, int pipe_read_end)
{
	if (mini->fdin != STDIN_FILENO)
		close(mini->fdin);
	mini->fdin = pipe_read_end;
	return (0);
}

static int	process_single_heredoc(t_mini *mini, t_token *token)
{
	int	pipefd[2];

	if (!token->next || !token->next->str)
		return (1);
	if (setup_heredoc_pipe(pipefd) != 0)
		return (1);
	signal(SIGINT, SIG_DFL);
	if (read_heredoc_lines(token->next->str, pipefd) != 0)
		return (cleanup_heredoc_resources(pipefd, 1));
	update_mini_fdin(mini, pipefd[0]);
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
