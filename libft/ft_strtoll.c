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

static int	set_base(const char **str, int *base)
{
	if (*base == 0)
	{
		if (**str == '0' && ((*str)[1] == 'x' || (*str)[1] == 'X'))
		{
			*base = 16;
			*str += 2;
		}
		else if (**str == '0')
		{
			*base = 8;
			(*str)++;
		}
		else
			*base = 10;
	}
	else if (*base == 16 && **str == '0' && ((*str)[1] == 'x'
		|| (*str)[1] == 'X'))
		*str += 2;
	return (1);
}

static int	get_digit_value(char c, int base)
{
	int	digit;

	digit = -1;
	if (ft_isdigit(c))
		digit = c - '0';
	else if (c >= 'a' && c <= 'z')
		digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z')
		digit = c - 'A' + 10;
	if (digit < 0 || digit >= base)
		return (-1);
	return (digit);
}

static long long	process_digits(const char **str, int base, int sign)
{
	long long	result;
	int			digit;

	result = 0;
	while (**str)
	{
		digit = get_digit_value(**str, base);
		if (digit == -1)
			break ;
		if (result > (LLONG_MAX - digit) / base)
		{
			if (sign == 1)
				return (LLONG_MAX);
			return (LLONG_MIN);
		}
		result = result * base + digit;
		(*str)++;
	}
	return (result);
}

long long	ft_strtoll(const char *str, char **endptr, int base)
{
	int			sign;
	long long	result;

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
	set_base(&str, &base);
	result = process_digits(&str, base, sign);
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}
