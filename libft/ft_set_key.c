/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:42:54 by aalegria          #+#    #+#             */
/*   Updated: 2025/04/03 18:42:56 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	try_set(int *found, char *to_set, void *value, enum e_types t)
{
	int	error;

	if (!*found)
		return ;
	error = ft_set_void(value, to_set, t);
	if (error)
		*found = -1;
}

int	ft_set_key(char **argv, char *searched_k, void *value, enum e_types t)
{
	char	*key;
	char	*to_set;
	int		found;
	size_t	i;

	if (!argv || !searched_k || !value)
		return (-1);
	key = NULL;
	to_set = NULL;
	found = 0;
	i = 0;
	while (argv[i] && found != -1 && found != 1)
	{
		found = !ft_key_value(argv[i], &key, &to_set, 0);
		if (found && key)
			found = !ft_strncmp(key, searched_k, -1);
		if (found && to_set)
			try_set(&found, to_set, value, t);
		if (key)
			(free(key), key = NULL);
		if (to_set && !(t == pntr && found == 1))
			(free(to_set), to_set = NULL);
		i++;
	}
	return (found);
}
