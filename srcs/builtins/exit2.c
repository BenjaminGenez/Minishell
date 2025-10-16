/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 13:52:38 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 13:52:41 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args, t_mini *mini)
{
	int	exit_code;

	if (check_too_many_args(args, mini))
		return (1);
	if (args[1] && !is_valid_number(args[1]))
		exit_code = handle_invalid_arg(args, mini);
	else if (args[1])
	{
		handle_numeric_arg(mini, args[1]);
		exit_code = mini->ret;
	}
	else
		exit_code = mini->ret;
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR);
	mini->exit = 1;
	return (exit_code);
}
