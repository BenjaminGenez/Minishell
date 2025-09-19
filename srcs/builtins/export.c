/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
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
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    
    return (1);
}

static void print_exported_vars(t_env *env)
{
    while (env)
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putstr_fd(env->key, STDOUT_FILENO);
        if (env->value)
        {
            ft_putstr_fd("=\"", STDOUT_FILENO);
            ft_putstr_fd(env->value, STDOUT_FILENO);
            ft_putstr_fd("\"", STDOUT_FILENO);
        }
        ft_putchar_fd('\n', STDOUT_FILENO);
        env = env->next;
    }
}

static int export_variable(t_shell *shell, char *arg)
{
    char    *equal_sign;
    char    *key;
    char    *value;
    int     ret;

    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
        return (0);
        
    key = ft_substr(arg, 0, equal_sign - arg);
    if (!key)
        return (1);
        
    value = ft_strdup(equal_sign + 1);
    if (!value)
    {
        free(key);
        return (1);
    }
    
    ret = set_env_value(&shell->env, key, value);
    free(key);
    free(value);
    
    return (ret);
}

int ft_export(t_shell *shell, t_cmd *cmd)
{
    int i;
    int status;
    int has_error;

    if (!cmd->args[1])
    {
        print_exported_vars(shell->env);
        return (0);
    }
    
    has_error = 0;
    i = 1;
    while (cmd->args[i])
    {
        if (!is_valid_identifier(cmd->args[i]))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd(cmd->args[i], STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            has_error = 1;
        }
        else if (ft_strchr(cmd->args[i], '='))
        {
            status = export_variable(shell, cmd->args[i]);
            if (status != 0)
                has_error = 1;
        }
        i++;
    }
    
    return (has_error ? 1 : 0);
}
