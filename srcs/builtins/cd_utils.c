/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 00:00:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/15 00:00:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_path_value(t_env *env, const char *var, size_t len)
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

int	handle_oldpwd_update(t_env *env, char *cwd)
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

void	print_cd_error(char *arg, int error_type)
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
