/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 21:55:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void set_env_var(t_env *env, const char *var, const char *value)
{
    char    *new_var;
    int     var_len;
    int     value_len;
    char    *new_value;

    var_len = ft_strlen(var);
    value_len = ft_strlen(value);
    new_var = (char *)malloc(var_len + value_len + 2);
    if (!new_var)
        return;
    ft_strlcpy(new_var, var, var_len + 1);
    new_var[var_len] = '=';
    ft_strlcpy(new_var + var_len + 1, value, value_len + 1);
    
    while (env)
    {
        if (ft_strncmp(env->value, var, var_len) == 0 && 
            (env->value[var_len] == '=' || env->value[var_len] == '\0'))
        {
            free(env->value);
            env->value = new_var;
            return;
        }
        if (!env->next)
            break;
        env = env->next;
    }
    
    // If we get here, the variable doesn't exist yet
    new_value = ft_strdup(new_var);
    free(new_var);
    if (!new_value)
        return;
        
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
    {
        free(new_value);
        return;
    }
    new_node->value = new_value;
    new_node->next = NULL;
    env->next = new_node;
}

static char	*extract_path_value(t_env *env, const char *var, size_t len)
{
	char	*path;
	int		i;
	int		s_alloc;
	char	*value_start;

	(void)var;
	(void)len;
	value_start = ft_strchr(env->value, '=');
	if (!value_start)
		return (ft_strdup(""));
	value_start++;
	s_alloc = ft_strlen(value_start);
	path = malloc(sizeof(char) * (s_alloc + 1));
	if (!path)
		return (NULL);
	i = 0;
	while (value_start[i])
	{
		path[i] = value_start[i];
		i++;
	}
	path[i] = '\0';
	return (path);
}

char	*get_env_path(t_env *env, const char *var, size_t len)
{
	while (env != NULL)
	{
		if (env->value && ft_strncmp(env->value, var, len)
			== 0 && env->value[len] == '=')
			return (extract_path_value(env, var, len));
		env = env->next;
	}
	return (NULL);
}

static int	handle_oldpwd_update(t_env *env, char *cwd)
{
	char	*oldpwd;

	oldpwd = get_env_path(env, "OLDPWD", 6);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	set_env_var(env, "OLDPWD", cwd);
	free(oldpwd);
	return (0);
}

int	update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (1);
	return (handle_oldpwd_update(env, cwd));
}

int	handle_path_option(int option, t_env *env, char **env_path)
{
	if (option == 1)
	{
		*env_path = get_env_path(env, "HOME", 4);
		if (!*env_path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR);
			return (1);
		}
	}
	else if (option == 2)
	{
		*env_path = get_env_path(env, "OLDPWD", 6);
		if (!*env_path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR);
			return (1);
		}
	}
	return (0);
}

static void	print_cd_error(char *arg, int error_type)
{
	ft_putstr_fd("minishell: cd: ", STDERR);
	if (error_type == 1)
		ft_putstr_fd("too many arguments\n", STDERR);
	else if (error_type == 2)
	{
		ft_putstr_fd(arg, STDERR);
		ft_putstr_fd(": ", STDERR);
		ft_putendl_fd(strerror(errno), STDERR);
	}
}

static int	update_pwd(t_env *env, char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd: error retrieving current directory");
		return (1);
	}
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", cwd);
	return (0);
}

static int	go_to_home(t_env *env, char *oldpwd)
{
	char	*home;

	home = get_env_path(env, "HOME", 4);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR);
		return (1);
	}
	if (chdir(home) == -1)
	{
		print_cd_error(home, 2);
		free(home);
		return (1);
	}
	free(home);
	return (update_pwd(env, oldpwd));
}

static int	go_to_oldpwd(t_env *env, char *oldpwd)
{
	char	*old_pwd;

	old_pwd = get_env_path(env, "OLDPWD", 6);
	if (!old_pwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR);
		return (1);
	}
	if (chdir(old_pwd) == -1)
	{
		print_cd_error(old_pwd, 2);
		free(old_pwd);
		return (1);
	}
	ft_putendl_fd(old_pwd, STDOUT);
	free(old_pwd);
	return (update_pwd(env, oldpwd));
}

int	go_to_path(char *path, t_env *env, char *oldpwd)
{
	if (chdir(path) == -1)
	{
		print_cd_error(path, 2);
		return (1);
	}
	return (update_pwd(env, oldpwd));
}

int	ft_cd(char **args, t_mini *mini)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	int		status;
	t_env	*env;

	env = mini->env;
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd: error retrieving current directory");
		return (1);
	}
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
	{
		perror("minishell: cd: malloc error");
		return (1);
	}
	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
		status = go_to_home(env, oldpwd);
	else if (ft_strcmp(args[1], "-") == 0)
		status = go_to_oldpwd(env, oldpwd);
	else
		status = go_to_path(args[1], env, oldpwd);
	if (status == 0)
		mini->exit = 0;
	free(oldpwd);
	return (status);
}
