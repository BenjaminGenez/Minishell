/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:09:38 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 19:09:38 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_heredoc_content(const char *delimiter, int pipefd[2])
{
    char *line;
    
    // Close the read end of the pipe, we only need the write end
    close(pipefd[0]);
    
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR);
            ft_putstr_fd(delimiter, STDERR);
            ft_putstr_fd("')\n", STDERR);
            break;
        }
        
        if (is_heredoc_delimiter(line, delimiter))
        {
            free(line);
            break;
        }
        
        // Write the line to the pipe
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    
    close(pipefd[1]);
    return (0);
}

char	*read_heredoc_input(const char *delimiter)
{
    char *line;
    char *content = ft_strdup("");
    char *temp;

    if (!content)
        return (NULL);

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR);
            ft_putstr_fd(delimiter, STDERR);
            ft_putstr_fd("')\n", STDERR);
            break;
        }

        if (is_heredoc_delimiter(line, delimiter))
        {
            free(line);
            break;
        }

        temp = ft_strjoin(content, line);
        free(content);
        content = temp;
        
        temp = ft_strjoin(content, "\n");
        free(content);
        content = temp;
        
        free(line);
    }

    return (content);
}
