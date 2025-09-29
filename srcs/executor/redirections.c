/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:46:11 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:46:11 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redirections(t_cmd *cmd)
{
    if (!cmd)
        return (1);

    // Redirigir entrada si es necesario
    if (cmd->in_fd != STDIN_FILENO)
    {
        if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2");
            return (0);
        }
        close(cmd->in_fd);
    }

    // Redirigir salida si es necesario
    if (cmd->out_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2");
            return (0);
        }
        close(cmd->out_fd);
    }

    return (1);
}

/*int handle_heredoc(char *delimiter)
{
    int     fd[2];
    char    *line;

    if (pipe(fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }

    close(fd[1]);
    return (fd[0]);
}
*/