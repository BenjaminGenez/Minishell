/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:58:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 15:58:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*handle_null_str(char c)
{
	char	*new_str;

	new_str = (char *)malloc(2 * sizeof(char));
	if (!new_str)
		return (NULL);
	new_str[0] = c;
	new_str[1] = '\0';
	return (new_str);
}

static int	copy_string(char *dest, char *src, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	return (i);
}

char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		str_len;
	int		i;

	if (!str)
		return (handle_null_str(c));
	str_len = ft_strlen(str);
	new_str = (char *)malloc((str_len + 2) * sizeof(char));
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	i = copy_string(new_str, str, str_len);
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}
