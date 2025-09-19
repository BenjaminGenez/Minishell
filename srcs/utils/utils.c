/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:42:03 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:42:03 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_cmds(t_cmd **cmds)
{
    t_cmd   *current;
    t_cmd   *next;
    int     i;

    if (!cmds || !*cmds)
        return ;
        
    current = *cmds;
    while (current)
    {
        next = current->next;
        
        if (current->args)
        {
            i = 0;
            while (current->args[i])
            {
                free(current->args[i]);
                i++;
            }
            free(current->args);
        }
        
        if (current->in_fd != STDIN_FILENO)
            close(current->in_fd);
        if (current->out_fd != STDOUT_FILENO)
            close(current->out_fd);
            
        free(current);
        current = next;
    }
    
    *cmds = NULL;
}

void    free_env(t_env **env)
{
    t_env   *current;
    t_env   *next;

    if (!env || !*env)
        return ;
        
    current = *env;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    
    *env = NULL;
}

void    handle_error(char *msg, int exit_status)
{
    if (msg)
        ft_putstr_fd(msg, STDERR_FILENO);
    exit(exit_status);
}

char    *ft_strjoin_free(char *s1, char *s2)
{
    char    *result;
    
    if (!s1 && !s2)
        return (NULL);
    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (ft_strdup(s1));
        
    result = ft_strjoin(s1, s2);
    free(s1);
    return (result);
}

int handle_heredoc(char *delimiter)
{
    int     fd[2];
    char    *line;
    
    if (pipe(fd) == -1)
        return (-1);
    
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
