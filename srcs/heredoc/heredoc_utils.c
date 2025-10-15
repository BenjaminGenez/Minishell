/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 00:00:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/15 00:00:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if token list contains heredoc
 */
int	contains_heredoc(t_token *tokens)
{
    t_token	*tmp;

    tmp = tokens;
    while (tmp)
    {
        if (tmp->type == HEREDOC)
            return (1);
        tmp = tmp->next;
    }
    return (0);
}

/**
 * @brief Check if line matches delimiter
 */
int	is_heredoc_delimiter(char *line, char *delimiter)
{
    if (!line || !delimiter)
        return (0);
    return (ft_strcmp(line, delimiter) == 0);
}