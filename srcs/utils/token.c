/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:35:49 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:36:07 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *next_sep(t_token *token, int skip)
{
    if (token && skip)
        token = token->next;
    while (token && token->type < TRUNC)
        token = token->next;
    return (token);
}

t_token *prev_sep(t_token *token, int skip)
{
    if (token && skip)
        token = token->prev;
    while (token && token->type < TRUNC)
        token = token->prev;
    return (token);
}

t_token *next_run(t_token *token, int skip)
{
    if (token && skip)
        token = token->next;
    while (token && token->type != CMD)
    {
        token = token->next;
        if (token && token->type == CMD && token->prev == NULL)
            ;
        else if (token && token->type == CMD && token->prev->type < END)
            token = token->next;
    }
    return (token);
}