/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:58:34 by aalegria          #+#    #+#             */
/*   Updated: 2024/09/20 11:54:25 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1) + 1;
	copy = (char *)malloc(len * sizeof(char));
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s1, len);
	return (copy);
}
