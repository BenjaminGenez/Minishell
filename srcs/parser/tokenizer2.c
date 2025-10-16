/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:39:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:39:02 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	next_alloc(char *line, int *i)
{
	int	count;

	count = 0;
	while (line[*i + count] && is_sep(line, *i + count) == 0)
	{
		if (!process_escaped_sequence(line, i, &count))
			count++;
	}
	return (count);
}
