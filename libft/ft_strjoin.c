/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:57:20 by aalegria          #+#    #+#             */
/*   Updated: 2024/09/20 11:57:25 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < s1_len)
	{
		new_str[i] = s1[i];
		i++;
	}
	while (i < s1_len + s2_len)
	{
		new_str[i] = s2[i - s1_len];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
