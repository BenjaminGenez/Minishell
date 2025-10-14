/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:43:54 by user              #+#    #+#             */
/*   Updated: 2025/10/09 12:48:04 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*ret;

	errno = 0;
	ret = getcwd(cwd, sizeof(cwd));
	if (ret != NULL)
	{
		ft_putendl_fd(cwd, STDOUT);
		if (errno != 0)
		{
			ft_putstr_fd("minishell: pwd: write error: ", STDERR);
			ft_putendl_fd(strerror(errno), STDERR);
			return (1);
		}
		return (0);
	}
	else
	{
		ft_putstr_fd("minishell: pwd: ", STDERR);
		ft_putendl_fd(strerror(errno), STDERR);
		return (1);
	}
}
