/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 23:00:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 23:00:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_char(int character)
{
	if (ft_isalnum(character) == 1 || character == '_')
		return (1);
	return (0);
}

int	is_valid_env(const char *env_str)
{
	int	idx;

	idx = 0;
	if (ft_isdigit(env_str[idx]) == 1)
		return (0);
	while (env_str[idx] && env_str[idx] != '=')
	{
		if (ft_isalnum(env_str[idx]) == 0)
			return (-1);
		idx++;
	}
	if (env_str[idx] != '=')
		return (2);
	return (1);
}
