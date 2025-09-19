/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:19:02 by aalegria          #+#    #+#             */
/*   Updated: 2024/09/26 15:21:07 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*get_next_word(char const **s, char c, int *len)
{
	while (**s && **s == c)
		(*s)++;
	*len = 0;
	while ((*s)[*len] && (*s)[*len] != c)
		(*len)++;
	if (*len == 0)
		return (NULL);
	return (ft_substr(*s, 0, (size_t)(*len)));
}

static void	ft_free_split(char **split, int i)
{
	while (i > 0)
	{
		free(split[i - 1]);
		i--;
	}
	free(split);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		word_count;
	int		i;
	int		len;

	if (!s)
		return (NULL);
	word_count = ft_count_words(s, c);
	result = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		result[i] = get_next_word(&s, c, &len);
		if (!result[i])
		{
			ft_free_split(result, i);
			return (NULL);
		}
		s += len;
		i++;
	}
	result[i] = NULL;
	return (result);
}
