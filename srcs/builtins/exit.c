/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_numeric(const char *str)
{
    int i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int ft_exit(t_shell *shell, t_cmd *cmd)
{
    int exit_code;
    int arg_count;

    arg_count = 0;
    while (cmd->args && cmd->args[arg_count])
        arg_count++;

    if (arg_count == 1)
        exit_code = shell->exit_status;
    else if (arg_count == 2 && is_numeric(cmd->args[1]))
        exit_code = ft_atoi(cmd->args[1]);
    else if (arg_count > 2 && is_numeric(cmd->args[1]))
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (1);
    }
    else
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        exit_code = 2;
    }

    // Limpiar y salir
    free_cmds(&shell->cmds);
    free_env(&shell->env);
    ft_putstr_fd("exit\n", STDERR_FILENO);
    exit(exit_code);
}
