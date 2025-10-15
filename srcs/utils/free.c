/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/10/03 18:43:02 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
void	free_token(t_token *token_list)
{
	while (token_list && token_list->next)
	{
		mem_free(token_list->str);
		token_list = token_list->next;
		mem_free(token_list->prev);
	}
	if (token_list)
	{
		mem_free(token_list->str);
		mem_free(token_list);
	}
}
void	free_env(t_env *env_list)
{
	t_env	*current_node;
	t_env	*next_node;

	if (!env_list)
		return ;
	current_node = env_list;
	while (current_node)
	{
		next_node = current_node->next;
		if (current_node->value)
			free(current_node->value);
		free(current_node);
		current_node = next_node;
	}
}
void	free_tab(char **string_array)
{
	int	idx;
	if (!string_array)
		return ;
	idx = 0;
	while (string_array[idx])
	{
		mem_free(string_array[idx]);
		idx++;
	}
	mem_free(string_array);
}
