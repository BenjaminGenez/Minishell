/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:25:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:25:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_node(t_mini *shell, t_env *node_to_remove)
{
	if (shell->env == node_to_remove && node_to_remove->next == NULL)
	{
		mem_free(shell->env->value);
		shell->env->value = NULL;
		shell->env->next = NULL;
		return ;
	}
	mem_free(node_to_remove->value);
	mem_free(node_to_remove);
}

int	ft_unset(char **cmd_args, t_mini *shell)
{
	int	i;

	if (!cmd_args[1])
		return (SUCCESS);
	i = 1;
	while (cmd_args[i])
	{
		unset_variable(cmd_args[i], shell);
		i++;
	}
	return (SUCCESS);
}
