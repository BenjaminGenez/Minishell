/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_valid_identifier(char *str)
{
    int i;

    if (!str || !*str || ft_isdigit(*str))
        return (0);
        
    i = 0;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    
    return (1);
}

int ft_unset(t_shell *shell, t_cmd *cmd)
{
    int i;
    int has_error;

    if (!cmd->args[1])
        return (0);
    
    has_error = 0;
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_identifier(cmd->args[i]))
        {
            ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
            ft_putstr_fd(cmd->args[i], STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            has_error = 1;
        }
        else
            unset_env(&shell->env, cmd->args[i]);
        i++;
    }
    
    return (has_error ? 1 : 0);
}
