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

int	update_pwd(t_env *env, char *oldpwd)
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

int	go_to_home(t_env *env, char *oldpwd)
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

int	go_to_oldpwd(t_env *env, char *oldpwd)
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

static int	cd_get_oldpwd(char **oldpwd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd: error retrieving current directory");
		return (1);
	}
	*oldpwd = ft_strdup(cwd);
	if (!*oldpwd)
	{
		perror("minishell: cd: malloc error");
		return (1);
	}
	return (0);
}

int	ft_cd(char **args, t_mini *mini)
{
	char	*oldpwd;
	int		status;
	t_env	*env;

	env = mini->env;
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR);
		return (1);
	}
	if (cd_get_oldpwd(&oldpwd) != 0)
		return (1);
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
