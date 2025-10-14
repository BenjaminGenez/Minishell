/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:25:00 by user              #+#    #+#             */
/*   Updated: 2025/10/08 14:25:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static int	is_space(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

long long	ft_strtoll(const char *str, char **endptr, int base)
{
	long long	result;
	int			sign;
	int			digit;

	if (base < 0 || base == 1 || base > 36)
	{
		if (endptr)
			*endptr = (char *)str;
		return (0);
	}
	while (is_space(*str))
		str++;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (base == 0)
	{
		if (*str == '0' && (str[1] == 'x' || str[1] == 'X'))
		{
			base = 16;
			str += 2;
		}
		else if (*str == '0')
		{
			base = 8;
			str++;
		}
		else
			base = 10;
	}
	else if (base == 16 && *str == '0' && (str[1] == 'x' || str[1] == 'X'))
		str += 2;
	result = 0;
	while (*str)
	{
		digit = -1;
		if (ft_isdigit(*str))
			digit = *str - '0';
		else if (*str >= 'a' && *str <= 'z')
			digit = *str - 'a' + 10;
		else if (*str >= 'A' && *str <= 'Z')
			digit = *str - 'A' + 10;
		if (digit < 0 || digit >= base)
			break ;
		if (result > (LLONG_MAX - digit) / base)
		{
			if (sign == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		result = result * base + digit;
		str++;
	}
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}
