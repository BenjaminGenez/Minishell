/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:51:36 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:51:38 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
