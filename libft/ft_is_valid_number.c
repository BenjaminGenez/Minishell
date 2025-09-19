/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_valid_number.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:02:42 by aalegria          #+#    #+#             */
/*   Updated: 2025/04/08 14:02:44 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_valid_number(const char *str)
{
	int	has_dot;

	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	has_dot = 0;
	while (*str)
	{
		if (*str == '.')
		{
			if (has_dot)
				return (0);
			has_dot = 1;
		}
		else if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}
