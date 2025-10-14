/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:09:38 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 19:09:38 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(t_mini *mini, t_token *token)
{
    char    *delimiter;
    int     pipefd[2];
    
    if (!token || !token->next)
        return (1);
        
    delimiter = token->next->str;
    
    if (pipe(pipefd) == -1)
    {
        perror("minishell: pipe");
        return (1);
    }
    
    // Set up signal handling for SIGINT during here-doc input
    signal(SIGINT, SIG_DFL);
    
    // Read the here-document content
    read_heredoc_content(delimiter, pipefd);
    
    // Set the input file descriptor to read from the pipe
    if (mini->fdin != STDIN_FILENO)
        close(mini->fdin);
    mini->fdin = pipefd[0];
    
    // Restore signal handler
    signal(SIGINT, SIG_DFL);
    
    return (0);
}

int	handle_heredoc_input(t_mini *mini, t_token *tokens)
{
	t_token *tmp = tokens;
	char *delimiter = NULL;
	char *line = NULL;
	int pipefd[2];
	
	// Find the here-document delimiter
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next)
		{
			delimiter = tmp->next->str;
			break;
		}
		tmp = tmp->next;
	}
	
	if (!delimiter)
		return (0);
	
	// Create a pipe for the here-document content
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	
	// Set up signal handling for SIGINT during here-doc input
	signal(SIGINT, SIG_DFL);
	
	// Read lines until the delimiter is found
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR);
			ft_putstr_fd(delimiter, STDERR);
			ft_putstr_fd("')\n", STDERR);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	
	close(pipefd[1]);
	
	// Restore default signal handler
	signal(SIGINT, SIG_DFL);
	
	// Set up the input for the command
	if (mini->fdin != STDIN)
		close(mini->fdin);
	mini->fdin = pipefd[0];
	
	return (0);
}
