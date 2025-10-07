/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_aux.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:14:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:14:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_path_value(t_env *env, const char *var, size_t len)
{
	char	*path;
	int		i;
	int		j;
	int		s_alloc;

	(void)var;
	s_alloc = ft_strlen(env->value) - len;
	path = malloc(sizeof(char) * (s_alloc + 1));
	if (!path)
		return (NULL);
	i = 0;
	j = 0;
	while (env->value[i++])
	{
		if (i > (int)len)
			path[j++] = env->value[i];
	}
	path[j] = '\0';
	return (path);
}

char	*get_env_path(t_env *env, const char *var, size_t len)
{
	while (env && env->next != NULL)
	{
		if (ft_strncmp(env->value, var, len) == 0)
			return (extract_path_value(env, var, len));
		env = env->next;
	}
	return (NULL);
}

static int	handle_oldpwd_update(t_env *env, char *cwd)
{
	char	*oldpwd;
	int		update_result;

	oldpwd = ft_strjoin("OLDPWD=", cwd);
	if (!oldpwd)
		return (ERROR);
	update_result = update_existing_var(env, oldpwd);
	if (update_result == 0)
		add_env_var(oldpwd, env);
	mem_free(oldpwd);
	return (SUCCESS);
}

int	update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (ERROR);
	return (handle_oldpwd_update(env, cwd));
}

int	handle_path_option(int option, t_env *env, char **env_path)
{
	if (option == 0)
	{
		update_oldpwd(env);
		*env_path = get_env_path(env, "HOME", 4);
		if (!*env_path)
		{
			ft_putendl_fd("minishell : cd: HOME not set", STDERR);
			return (ERROR);
		}
	}
	else if (option == 1)
	{
		*env_path = get_env_path(env, "OLDPWD", 6);
		if (!*env_path)
		{
			ft_putendl_fd("minishell : cd: OLDPWD not set", STDERR);
			return (ERROR);
		}
		update_oldpwd(env);
	}
	return (SUCCESS);
}
