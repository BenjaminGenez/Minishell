/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:28:47 by aalegria          #+#    #+#             */
/*   Updated: 2024/09/23 16:31:43 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Escribe el carácter 'c' en el descriptor de archivo 'fd'.
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
