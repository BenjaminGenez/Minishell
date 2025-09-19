/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod_s.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:39:28 by aalegria          #+#    #+#             */
/*   Updated: 2025/04/03 19:41:52 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_str(char *str, int *error)
{
	size_t	i;
	int		is_valid;

	if (!error || !str || !*str)
		return (1);
	i = 0;
	is_valid = ft_isdigit(str[i]) || str[i] == '.' || str[i] == '-';
	while (str[i] && is_valid)
	{
		is_valid = ft_isdigit(str[i]) || str[i] == '.'
			|| (i == 0 && str[i] == '-');
		i++;
	}
	return (!is_valid || i == 0);
}

static double	get_decimal(char *dot_pos)
{
	double	out;
	size_t	zeros;
	size_t	len;

	out = 0;
	if (dot_pos)
	{
		out = ft_atoi(dot_pos + 1);
		zeros = 0;
		while (dot_pos[zeros + 1] == '0')
			zeros++;
		len = zeros;
		if (out)
			len += ft_strlen(dot_pos + zeros + 1);
		while (len--)
			out /= 10;
		*dot_pos = 0;
	}
	return (out);
}

double	ft_atod_s(char *str, int *error)
{
	char	*dot_pos;
	double	result;
	char	*str_dup;
	double	integer;

	*error = 0;
	if (check_str(str, error))
		return (*error = 1, 0);
	dot_pos = ft_strchr(str, '.');
	if (dot_pos && ft_strchr(dot_pos + 1, '.'))
		return (*error = 1, 0);
	str_dup = ft_strdup(str);
	if (!str_dup)
		return (*error = 1, 0);
	dot_pos = ft_strchr(str_dup, '.');
	result = get_decimal(dot_pos);
	integer = ft_atoi(str_dup);
	if (integer < 0 || (ft_strlen(str_dup) == 1 && str_dup[0] == '-'))
		result *= -1;
	result += integer;
	free(str_dup);
	return (result);
}
