/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/10/10 15:17:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_signal;

static void	init_shell(t_mini *shell)
{
	ft_bzero(shell, sizeof(t_mini));
	shell->in = STDIN;
	shell->out = STDOUT;
	shell->fdin = -1;
	shell->fdout = -1;
	shell->pipin = -1;
	shell->pipout = -1;
	shell->pid = -1;
	shell->exit = 0;
	shell->ret = 0;
	shell->no_exec = 0;
	shell->saved_stdout = -1;
	sig_init();
	signal(SIGINT, sig_int);
	signal(SIGQUIT, SIG_IGN);
}

static int	setup_env(t_mini *shell, char **env)
{
	if (setup_env_list(shell, env) != 0)
		return (1);
	increment_shell_level(shell->env);
	return (0);
}

void	cleanup_shell(t_mini *shell)
{
	if (!shell)
		return ;
	cleanup_tokens_env(shell);
	if (shell->fdin >= 0)
		close(shell->fdin);
	if (shell->fdout >= 0)
		close(shell->fdout);
	if (shell->pipin >= 0 && shell->pipin != STDIN)
		close(shell->pipin);
	if (shell->pipout >= 0 && shell->pipout != STDOUT)
		close(shell->pipout);
	if (shell->saved_stdout >= 0)
		close(shell->saved_stdout);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	int		ret;

	(void)argc;
	(void)argv;
	init_shell(&shell);
	if (setup_env(&shell, env) != 0)
		return (1);
	input_loop(&shell);
	ret = shell.ret;
	cleanup_shell(&shell);
	return (ret);
}
