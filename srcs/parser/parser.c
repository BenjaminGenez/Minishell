/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:50 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:39:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_cmd *init_command(void)
{
    t_cmd *cmd;

    cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    
    cmd->args = NULL;
    cmd->in_fd = STDIN_FILENO;
    cmd->out_fd = STDOUT_FILENO;
    cmd->next = NULL;
    
    return (cmd);
}

static int add_arg(t_cmd *cmd, char *arg)
{
    char    **new_args;
    int     i;

    if (!arg)
        return (1);
    
    i = 0;
    if (cmd->args)
        while (cmd->args[i])
            i++;
            
    new_args = (char **)malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return (0);
        
    i = 0;
    if (cmd->args)
    {
        while (cmd->args[i])
        {
            new_args[i] = cmd->args[i];
            i++;
        }
        free(cmd->args);
    }
    
    new_args[i] = arg;
    new_args[i + 1] = NULL;
    cmd->args = new_args;
    return (1);
}

t_cmd *parse_pipeline(t_token **tokens)
{
    t_cmd   *pipeline;
    t_cmd   *current;
    t_token *tmp;

    if (!tokens || !*tokens)
        return (NULL);
        
    pipeline = parse_command(tokens);
    if (!pipeline)
        return (NULL);
        
    current = pipeline;
    while (*tokens && (*tokens)->type == TOKEN_PIPE)
    {
        tmp = *tokens;
        *tokens = (*tokens)->next;
        free(tmp->value);
        free(tmp);
        
        current->next = parse_command(tokens);
        if (!current->next)
        {
            free_cmds(&pipeline);
            return (NULL);
        }
        current = current->next;
    }
    
    return (pipeline);
}

t_cmd *parse_command(t_token **tokens)
{
    t_cmd   *cmd;
    int     status;

    if (!tokens || !*tokens)
        return (NULL);
        
    cmd = init_command();
    if (!cmd)
        return (NULL);
        
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if (is_redirection((*tokens)->type))
        {
            status = parse_redirections(cmd, tokens);
            if (!status)
            {
                free_cmds(&cmd);
                return (NULL);
            }
        }
        else
        {
            if (!add_arg(cmd, ft_strdup((*tokens)->value)))
            {
                free_cmds(&cmd);
                return (NULL);
            }
            free((*tokens)->value);
            free(*tokens);
            *tokens = (*tokens)->next;
        }
    }
    
    return (cmd);
}

int parse_redirections(t_cmd *cmd, t_token **tokens)
{
    t_token_type type;
    char        *filename;
    int         *fd;

    if (!cmd || !tokens || !*tokens)
        return (0);
        
    type = (*tokens)->type;
    free((*tokens)->value);
    free(*tokens);
    *tokens = (*tokens)->next;
    
    if (!*tokens || (*tokens)->type != TOKEN_WORD)
        return (0);
        
    filename = (*tokens)->value;
    (*tokens)->value = NULL;
    free(*tokens);
    *tokens = (*tokens)->next;
    
    if (type == TOKEN_REDIR_IN)
        fd = &cmd->in_fd;
    else if (type == TOKEN_REDIR_OUT)
        fd = &cmd->out_fd;
    else if (type == TOKEN_APPEND)
    {
        *fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        free(filename);
        return (*fd != -1);
    }
    else if (type == TOKEN_HEREDOC)
    {
        *fd = handle_heredoc(filename);
        free(filename);
        return (*fd != -1);
    }
    else
    {
        free(filename);
        return (0);
    }
    
    *fd = open(filename, 
              (type == TOKEN_REDIR_IN) ? O_RDONLY : (O_WRONLY | O_CREAT | O_TRUNC),
              0644);
    free(filename);
    
    return (*fd != -1);
}

int parse_input(t_shell *shell, char *input)
{
    t_token *tokens;
    t_cmd   *cmds;

    (void)shell; // Se usará más adelante para la expansión de variables
    
    if (!input || !*input)
        return (1);
        
    tokens = tokenize(input);
    if (!tokens)
        return (1);
        
    cmds = parse_pipeline(&tokens);
    if (!cmds)
    {
        free_tokens(&tokens);
        return (1);
    }
    
    shell->cmds = cmds;
    free_tokens(&tokens);
    return (0);
}
