/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:09:38 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 19:09:38 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_content(t_mini *mini, char *delimiter, int pipefd[2])
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
			break ;
		}
		if (is_heredoc_delimiter(line, delimiter))
		{
			free(line);
			return (0);
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	return (1);
}

char	*process_heredoc(char *input)
{
    char *heredoc_pos;
    char *delimiter_start;
    char *delimiter_end;
    char *delimiter;
    char *content;
    char *new_input;
    char *before_heredoc;
    char *after_heredoc;
    size_t before_len;

    heredoc_pos = ft_strstr(input, "<<");
    if (!heredoc_pos)
        return (input);

    // Get the part before the here-document
    before_len = heredoc_pos - input;
    before_heredoc = ft_strndup(input, before_len);
    if (!before_heredoc)
        return (input);

    // Find the delimiter
    delimiter_start = heredoc_pos + 2; // Skip "<<"
    while (*delimiter_start && ft_isspace(*delimiter_start))
        delimiter_start++;
    
    if (!*delimiter_start)
    {
        free(before_heredoc);
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR);
        return (input);
    }

    // Find the end of the delimiter
    delimiter_end = delimiter_start;
    while (*delimiter_end && !ft_isspace(*delimiter_end) && 
           *delimiter_end != '|' && *delimiter_end != ';' && 
           *delimiter_end != '&' && *delimiter_end != '<' && 
           *delimiter_end != '>')
        delimiter_end++;

    // Extract the delimiter
    delimiter = ft_strndup(delimiter_start, delimiter_end - delimiter_start);
    if (!delimiter)
    {
        free(before_heredoc);
        perror("minishell: malloc");
        return (input);
    }

    // Read the here-document content
    content = read_heredoc_input(delimiter);
    free(delimiter);
    
    if (!content)
    {
        free(before_heredoc);
        return (input);
    }

    // Get the part after the delimiter
    after_heredoc = ft_strdup(delimiter_end);
    if (!after_heredoc)
    {
        free(before_heredoc);
        free(content);
        return (input);
    }

    // Combine everything
    new_input = ft_strjoin(before_heredoc, content);
    free(before_heredoc);
    free(content);
    
    if (!new_input)
    {
        free(after_heredoc);
        return (input);
    }
    
    char *temp = ft_strjoin(new_input, after_heredoc);
    free(new_input);
    free(after_heredoc);
    
    if (!temp)
        return (input);

    free(input);
    return (temp);
}
