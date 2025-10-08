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
	{
		ft_putendl_fd("DEBUG: count_args: token is NULL", STDERR);
		return (0);
	}
	count = 1;
	token = token->next;
	while (token && token->type < TRUNC)
	{
		count++;
		token = token->next;
	}
	ft_putstr_fd("DEBUG: count_args: Found ", STDERR);
	ft_putnbr_fd(count, STDERR);
	ft_putendl_fd(" arguments", STDERR);
	return (count);
}

static void	fill_args_array(char **args_array, t_token *token)
{
	int	i;

	ft_putendl_fd("DEBUG: fill_args_array: Starting", STDERR);
	if (!args_array || !token)
	{
		ft_putendl_fd("DEBUG: fill_args_array: NULL args_array or token", STDERR);
		return ;
	}

	i = 0;
	ft_putstr_fd("DEBUG: fill_args_array: Processing token ", STDERR);
	ft_putnbr_fd(i, STDERR);
	ft_putstr_fd(": ", STDERR);
	if (token->str)
	{
		ft_putstr_fd("\"", STDERR);
		ft_putstr_fd(token->str, STDERR);
		ft_putstr_fd("\"", STDERR);
	}
	else
	{
		ft_putstr_fd("(null)", STDERR);
	}
	ft_putchar_fd('\n', STDERR);

	args_array[i] = ft_strdup(token->str);
	if (!args_array[i])
	{
		ft_putendl_fd("DEBUG: fill_args_array: strdup failed for token", STDERR);
		free_tab(args_array);
		return ;
	}

	i++;
	token = token->next;
	while (token && token->type < TRUNC)
	{
		ft_putstr_fd("DEBUG: fill_args_array: Processing token ", STDERR);
		ft_putnbr_fd(i, STDERR);
		ft_putstr_fd(": ", STDERR);
		if (token->str)
		{
			ft_putstr_fd("\"", STDERR);
			ft_putstr_fd(token->str, STDERR);
			ft_putstr_fd("\"", STDERR);
		}
		else
		{
			ft_putstr_fd("(null)", STDERR);
		}
		ft_putchar_fd('\n', STDERR);

		args_array[i] = ft_strdup(token->str);
		if (!args_array[i])
		{
			ft_putendl_fd("DEBUG: fill_args_array: strdup failed for token", STDERR);
			free_tab(args_array);
			return ;
		}
		i++;
		token = token->next;
	}
	args_array[i] = NULL;

	// Print the final args array for debugging
	ft_putendl_fd("DEBUG: fill_args_array: Final args array:", STDERR);
	for (int j = 0; args_array[j]; j++)
	{
		ft_putstr_fd("  - args_array[", STDERR);
		ft_putnbr_fd(j, STDERR);
		ft_putstr_fd("] = \"", STDERR);
		ft_putstr_fd(args_array[j], STDERR);
		ft_putendl_fd("\"", STDERR);
	}
}

char	**build_cmd_array(t_token *start_token)
{
	char	**args_array;
	int		arg_count;

	ft_putendl_fd("\nDEBUG: build_cmd_array: Starting", STDERR);
	if (!start_token)
	{
		ft_putendl_fd("DEBUG: build_cmd_array: start_token is NULL", STDERR);
		return (NULL);
	}

	arg_count = count_args(start_token);
	ft_putstr_fd("DEBUG: build_cmd_array: Allocating ", STDERR);
	ft_putnbr_fd(arg_count + 1, STDERR);
	ft_putendl_fd(" elements for args_array", STDERR);

	args_array = malloc(sizeof(char *) * (arg_count + 1));
	if (!args_array)
	{
		ft_putendl_fd("DEBUG: build_cmd_array: malloc failed for args_array", STDERR);
		return (NULL);
	}

	fill_args_array(args_array, start_token);
	
	ft_putstr_fd("DEBUG: build_cmd_array: Returning args_array at ", STDERR);
	ft_putnbr_fd((long)args_array, 16);
	ft_putchar_fd('\n', STDERR);
	
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
