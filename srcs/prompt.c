/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:38:07 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_prompt(void)
{
    char    *cwd;
    char    *prompt;
    char    *tmp;
    char    *user;
    
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (ft_strdup("minishell> "));
    
    user = getenv("USER");
    if (!user)
        user = "user";
    
    tmp = ft_strjoin(user, "@");
    if (!tmp)
    {
        free(cwd);
        return (ft_strdup("minishell> "));
    }
    
    prompt = ft_strjoin(tmp, " ");
    free(tmp);
    if (!prompt)
    {
        free(cwd);
        return (ft_strdup("minishell> "));
    }
    
    tmp = ft_strjoin(prompt, cwd);
    free(prompt);
    free(cwd);
    if (!tmp)
        return (ft_strdup("minishell> "));
    
    prompt = ft_strjoin(tmp, "$ ");
    free(tmp);
    if (!prompt)
        return (ft_strdup("minishell> "));
    
    return (prompt);
}
