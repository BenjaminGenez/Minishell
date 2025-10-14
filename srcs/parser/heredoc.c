/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 20:50:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 21:35:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads the content for a here-document until the delimiter is found.
 * 
 * @param delimiter The delimiter that marks the end of the here-document
 * @param pipefd Pipe file descriptors for writing the content
 * @return int 0 on success, 1 on error
 */
static int	read_heredoc_impl(const char *delimiter, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file "
				"(wanted `", STDERR_FILENO);
			ft_putstr_fd((char *)delimiter, STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
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

/**
 * @brief Handles a single here-document token.
 * 
 * @param mini The main shell structure
 * @param token The here-document token
 * @return int 0 on success, 1 on error
 */
static int	process_heredoc_impl(t_mini *mini, t_token *token)
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
	if (read_heredoc_impl(token->next->str, pipefd) != 0)
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

/**
 * @brief Checks if the token list contains any here-document tokens.
 * 
 * @param tokens The token list to check
 * @return int 1 if a heredoc token is found, 0 otherwise
 */
static int	has_heredoc_token(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/**
 * @brief Processes all here-document tokens in the token list.
 * 
 * @param mini The main shell structure
 */
static void	process_heredoc_tokens(t_mini *mini)
{
	t_token	*tmp;

	tmp = mini->start;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (process_heredoc_impl(mini, tmp) != 0)
				break ;
		}
		tmp = tmp->next;
	}
}

/* ======================== EXPORTED FUNCTIONS =========================== */

int	read_heredoc_content(const char *delimiter, int pipefd[2])
{
	return (read_heredoc_impl(delimiter, pipefd));
}

int	handle_heredoc(t_mini *mini, t_token *token)
{
	return (process_heredoc_impl(mini, token));
}

int	contains_heredoc(t_token *tokens)
{
	return (has_heredoc_token(tokens));
}

void	process_tokens_heredoc(t_mini *mini)
{
	process_heredoc_tokens(mini);
}
