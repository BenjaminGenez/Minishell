/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:45:00 by user              #+#    #+#             */
/*   Updated: 2025/10/07 17:45:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(t_token *token)
{
	int	count;

	if (!token)
		return (0);
	count = 1;
	token = token->next;
	while (token && token->type < TRUNC)
	{
		count++;
		token = token->next;
	}
	return (count);
}

static void	fill_args_array(char **args_array, t_token *token)
{
	int	i;

	if (!args_array || !token)
		return ;
	i = 0;
	args_array[i++] = token->str;
	token = token->next;
	while (token && token->type < TRUNC)
	{
		args_array[i] = token->str;
		i++;
		token = token->next;
	}
	args_array[i] = NULL;
}

char	**build_cmd_array(t_token *start_token)
{
	char	**args_array;
	int		arg_count;

	if (!start_token)
		return (NULL);
	arg_count = count_args(start_token);
	args_array = malloc(sizeof(char *) * (arg_count + 1));
	if (!args_array)
		return (NULL);
	fill_args_array(args_array, start_token);
	return (args_array);
}

void	cleanup_shell_fds(t_mini *shell)
{
	ft_close(shell->pipin);
	ft_close(shell->pipout);
	shell->pipin = -1;
	shell->pipout = -1;
	shell->charge = 0;
}
