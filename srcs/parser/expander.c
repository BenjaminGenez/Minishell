/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:42:03 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:42:03 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char *get_env_value_str(t_shell *shell, char *key)
{
    char    *value;
    char    *result;
    
    if (!key || !*key)
        return (ft_strdup("$"));
        
    if (ft_strcmp(key, "?") == 0)
    {
        result = ft_itoa(shell->exit_status);
        return (result ? result : ft_strdup(""));
    }
    
    value = get_env_value(shell->env, key);
    if (value)
        return (ft_strdup(value));
    return (ft_strdup(""));
}

static char *handle_dollar(t_shell *shell, char *str, int *i)
{
    char    *var_name;
    char    *var_value;
    char    *result;
    int     start;
    
    (*i)++;
    start = *i;
    
    if (str[*i] == '?')
    {
        (*i)++;
        return (get_env_value_str(shell, "?"));
    }
    
    if (!ft_isalnum(str[*i]) && str[*i] != '_')
        return (ft_strdup("$"));
        
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        (*i)++;
    
    var_name = ft_substr(str, start, *i - start);
    if (!var_name)
        return (NULL);
        
    var_value = get_env_value_str(shell, var_name);
    free(var_name);
    
    return (var_value);
}

static char *handle_quotes(t_shell *shell, char *str, int *i, char quote)
{
    char    *result;
    char    *tmp;
    int     start;
    
    (*i)++;
    start = *i;
    
    while (str[*i] && str[*i] != quote)
    {
        if (quote == '"' && str[*i] == '$' && str[*i + 1] && 
            (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_' || str[*i + 1] == '?'))
        {
            tmp = ft_substr(str, start, *i - start);
            result = ft_strjoin_free(result, tmp);
            free(tmp);
            
            tmp = handle_dollar(shell, str, i);
            result = ft_strjoin_free(result, tmp);
            free(tmp);
            start = *i;
        }
        else
            (*i)++;
    }
    
    tmp = ft_substr(str, start, *i - start);
    result = ft_strjoin_free(result, tmp);
    free(tmp);
    
    if (str[*i] == quote)
        (*i)++;
        
    return (result);
}

char *expand_env_vars(t_shell *shell, char *str)
{
    char    *result;
    char    *tmp;
    int     i;
    int     start;
    
    if (!str)
        return (NULL);
        
    result = ft_strdup("");
    i = 0;
    start = 0;
    
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            tmp = ft_substr(str, start, i - start);
            result = ft_strjoin_free(result, tmp);
            free(tmp);
            
            tmp = handle_quotes(shell, str, &i, str[i]);
            result = ft_strjoin_free(result, tmp);
            free(tmp);
            
            start = i;
        }
        else if (str[i] == '$' && str[i + 1] && 
                (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
        {
            tmp = ft_substr(str, start, i - start);
            result = ft_strjoin_free(result, tmp);
            free(tmp);
            
            tmp = handle_dollar(shell, str, &i);
            result = ft_strjoin_free(result, tmp);
            free(tmp);
            
            start = i;
        }
        else
            i++;
    }
    
    tmp = ft_substr(str, start, i - start);
    result = ft_strjoin_free(result, tmp);
    free(tmp);
    
    return (result);
}

int expand_variables(t_shell *shell, t_cmd *cmd)
{
    int     i;
    char    *expanded;
    
    if (!cmd || !cmd->args)
        return (1);
        
    i = 0;
    while (cmd->args[i])
    {
        expanded = expand_env_vars(shell, cmd->args[i]);
        if (!expanded)
            return (0);
            
        free(cmd->args[i]);
        cmd->args[i] = expanded;
        i++;
    }
    
    return (1);
}
