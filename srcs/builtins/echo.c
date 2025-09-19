/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_echo(t_cmd *cmd)
{
    int i;
    int newline;

    if (!cmd || !cmd->args)
        return (1);
    
    i = 1;
    newline = 1;
    
    // Verificar la opción -n
    while (cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
    {
        int j = 1;
        while (cmd->args[i][j] == 'n')
            j++;
        if (cmd->args[i][j] == '\0')
        {
            newline = 0;
            i++;
        }
        else
            break;
    }
    
    // Imprimir los argumentos
    while (cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    
    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    return (0);
}
