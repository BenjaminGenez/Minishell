/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:12:50 by aalegria          #+#    #+#             */
/*   Updated: 2024/09/25 13:13:44 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Función que maneja el signo (+ o -) y ajusta el índice de la cadena */
static int	ft_check_sign(const char **str)
{
	int	sign;

	sign = 1;
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

int	ft_atoi(const char *str)
{
	int		result;
	int		sign;

	result = 0;
	sign = ft_check_sign(&str);
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
