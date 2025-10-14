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

/* Helper function to free a string array */
static void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	count_args(t_token *token)
{
	int	count;

	if (!token)
	{
		return (0);
	}
	count = 1;
	token = token->next;
	while (token && token->type < TRUNC)
	{
		if (token->type != TRUNC && token->type != APPEND
			&& token->type != INPUT && token->type != HEREDOC)
		{
			count++;
		}
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
	args_array[i] = strdup(token->str);
	if (!args_array[i])
	{
		free_str_array(args_array);
		return ;
	}
	i++;
	token = token->next;
	while (token && token->type < TRUNC)
	{
		if (token->type == TRUNC || token->type == APPEND
			|| token->type == INPUT || token->type == HEREDOC)
		{
			token = token->next;
			if (token)
				token = token->next;
		}
		else
		{
			args_array[i] = ft_strdup(token->str);
			if (!args_array[i])
			{
				free_str_array(args_array);
				return ;
			}
			i++;
			token = token->next;
		}
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
	if (shell->pipin != -1)
	{
		close(shell->pipin);
		shell->pipin = -1;
	}
	if (shell->pipout != -1)
	{
		close(shell->pipout);
		shell->pipout = -1;
	}
	shell->charge = 0;
}
