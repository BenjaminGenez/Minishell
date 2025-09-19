/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:43:54 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_home_path(t_shell *shell)
{
    char *home;
    
    home = get_env_value(shell->env, "HOME");
    if (!home)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
        return (NULL);
    }
    return (home);
}

static int update_pwd_vars(t_shell *shell)
{
    char    cwd[PATH_MAX];
    char    *oldpwd;
    
    if (!getcwd(cwd, PATH_MAX))
    {
        perror("minishell: cd: error getting current directory");
        return (1);
    }
    
    oldpwd = get_env_value(shell->env, "PWD");
    if (oldpwd)
    {
        if (set_env_value(&shell->env, "OLDPWD", oldpwd) != 0)
            return (1);
    }
    
    return (set_env_value(&shell->env, "PWD", cwd));
}

int ft_cd(t_shell *shell, t_cmd *cmd)
{
    char    *path;
    int     ret;
    
    if (!cmd->args || !cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
    {
        path = get_home_path(shell);
        if (!path)
            return (1);
    }
    else if (ft_strcmp(cmd->args[1], "-") == 0)
    {
        path = get_env_value(shell->env, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            return (1);
        }
        ft_putendl_fd(path, STDOUT_FILENO);
    }
    else
        path = cmd->args[1];
    
    ret = chdir(path);
    if (ret != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        if (access(path, F_OK) == -1)
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        else if (access(path, R_OK) == -1)
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        else
            ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
        return (1);
    }
    
    return (update_pwd_vars(shell));
}
