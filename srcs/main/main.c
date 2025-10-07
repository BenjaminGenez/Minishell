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

static void	cleanup_shell(t_mini *shell)
{
	free_env(shell->env);
	free_env(shell->secret_env);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell);
	if (setup_env(&shell, env) != 0)
		return (1);
	while (shell.exit == 0)
	{
		sig_init();
		parse(&shell);
		if (shell.start != NULL && check_line(&shell, shell.start))
			exec_pipeline(&shell);
		free_token(shell.start);
	}
	cleanup_shell(&shell);
	return (shell.ret);
}
