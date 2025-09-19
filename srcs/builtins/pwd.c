/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(void)
{
    char    cwd[PATH_MAX];
    
    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        perror("minishell: pwd");
        return (1);
    }
    
    ft_putendl_fd(cwd, STDOUT_FILENO);
    return (0);
}
