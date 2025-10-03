/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:46:07 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:46:09 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd_name)
{
    const char *builtins[] = {"echo", "cd", "pwd", "env", "export", "unset", NULL};
    int idx;

    if (!cmd_name)
        return (0);
    idx = 0;
    while (builtins[idx])
    {
        if (ft_strcmp(cmd_name, builtins[idx]) == 0)
            return (1);
        idx++;
    }
    return (0);
}

int exec_builtin(char **cmd_args, t_mini *shell)
{
    int exit_code;
    char *cmd;

    exit_code = 0;
    cmd = cmd_args[0];
    if (ft_strcmp(cmd, "echo") == 0)
        exit_code = ft_echo(cmd_args);
    else if (ft_strcmp(cmd, "cd") == 0)
        exit_code = ft_cd(cmd_args, shell->env);
    else if (ft_strcmp(cmd, "pwd") == 0)
        exit_code = ft_pwd();
    else if (ft_strcmp(cmd, "env") == 0)
        ft_env(shell->env);
    else if (ft_strcmp(cmd, "export") == 0)
        ft_export(cmd_args, shell->env, shell->secret_env);
    else if (ft_strcmp(cmd, "unset") == 0)
        ft_unset(cmd_args, shell);
    return (exit_code);
}