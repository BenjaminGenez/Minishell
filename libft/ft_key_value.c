/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:29:21 by aalegria          #+#    #+#             */
/*   Updated: 2025/04/03 19:44:16 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_str_cond(char *str, int free_str)
{
	if (free_str)
		free(str);
}

static int	validate_key_value(char *str, size_t del_pntr, size_t str_end)
{
	if (!str)
		return (1);
	if (del_pntr == (size_t)str || del_pntr == str_end || !del_pntr)
		return (1);
	return (0);
}

static void	free_key_value(char **key, char **value)
{
	free(*key);
	free(*value);
	*key = NULL;
	*value = NULL;
}

int	extract_key_value(char *str, char **key, char **value, size_t eq_pos)
{
	size_t	key_len;

	key_len = eq_pos - (size_t)str;
	*key = ft_substr(str, 0, key_len);
	*value = ft_substr((char *)eq_pos + 1, 0, -1);
	return (!*key || !*value);
}

int	ft_key_value(char *str, char **key, char **value, int free_str)
{
	size_t	eq_pos;
	size_t	str_end;

	*key = NULL;
	*value = NULL;
	eq_pos = (size_t)ft_strchr(str, '=');
	str_end = (size_t)ft_strchr(str, 0);
	if (validate_key_value(str, eq_pos, str_end))
	{
		free_str_cond(str, free_str);
		return (1);
	}
	if (extract_key_value(str, key, value, eq_pos))
	{
		free_key_value(key, value);
		free_str_cond(str, free_str);
		return (1);
	}
	free_str_cond(str, free_str);
	return (0);
}
