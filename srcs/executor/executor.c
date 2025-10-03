/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:46:20 by user              #+#    #+#             */
/*   Updated: 2025/10/03 23:35:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **build_cmd_array(t_token *start_token)
{
    t_token *current;
    char **args_array;
    int arg_count;

    if (!start_token)
        return (NULL);
    current = start_token->next;
    arg_count = 2;
    while (current && current->type < TRUNC)
    {
        current = current->next;
        arg_count++;
    }
    if (!(args_array = malloc(sizeof(char *) * arg_count)))
        return (NULL);
    current = start_token->next;
    args_array[0] = start_token->str;
    arg_count = 1;
    while (current && current->type < TRUNC)
    {
        args_array[arg_count++] = current->str;
        current = current->next;
    }
    args_array[arg_count] = NULL;
    return (args_array);
}

void exec_cmd(t_mini *shell, t_token *token)
{
    char **cmd_args;
    int idx;

    if (shell->charge == 0)
        return;
    cmd_args = build_cmd_array(token);
    idx = 0;
    while (cmd_args && cmd_args[idx])
    {
        cmd_args[idx] = expansions(cmd_args[idx], shell->env, shell->ret);
        idx++;
    }
    if (cmd_args && ft_strcmp(cmd_args[0], "exit") == 0 && has_pipe(token) == 0)
        mini_exit(shell, cmd_args);
    else if (cmd_args && is_builtin(cmd_args[0]))
        shell->ret = exec_builtin(cmd_args, shell);
    else if (cmd_args)
        shell->ret = exec_bin(cmd_args, shell->env, shell);
    free_tab(cmd_args);
    ft_close(shell->pipin);
    ft_close(shell->pipout);
    shell->pipin = -1;
    shell->pipout = -1;
    shell->charge = 0;
}