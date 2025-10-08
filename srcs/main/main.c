/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/10/07 14:32:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

static void	init_shell(t_mini *shell)
{
	ft_bzero(shell, sizeof(t_mini));
	shell->in = dup(STDIN);
	shell->out = dup(STDOUT);
	shell->exit = 0;
	shell->ret = 0;
	shell->no_exec = 0;
	reset_fds(shell);
}

static int	setup_env(t_mini *shell, char **env)
{
	if (setup_env_list(shell, env) != 0)
		return (1);
	if (setup_secret_env(shell, env) != 0)
		return (1);
	increment_shell_level(shell->env);
	return (0);
}

void	cleanup_shell(t_mini *shell)
{
	if (shell->env)
		free_env(shell->env);
	if (shell->secret_env)
		free_env(shell->secret_env);
	if (shell->start)
		free_token(shell->start);
	reset_fds(shell);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;

	(void)argc;
	(void)argv;
	ft_putendl_fd("DEBUG: Starting minishell...", STDERR);
	init_shell(&shell);
	if (setup_env(&shell, env) != 0)
	{
		ft_putendl_fd("DEBUG: Failed to setup environment", STDERR);
		return (1);
	}
	ft_putendl_fd("DEBUG: Minishell initialized successfully", STDERR);
	while (shell.exit == 0)
	{
		sig_init();
		ft_putendl_fd("DEBUG: Waiting for input...", STDERR);
		parse(&shell);
		if (shell.start != NULL && check_line(&shell, shell.start))
			exec_pipeline(&shell);
		free_token(shell.start);
	}
	ft_putendl_fd("DEBUG: Cleaning up and exiting...", STDERR);
	cleanup_shell(&shell);
	return (shell.ret);
}
