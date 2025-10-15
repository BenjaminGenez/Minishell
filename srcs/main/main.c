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

#define GLOBAL_VARIABLE_DEFINITION
#include "minishell.h"

static void	execute_with_pipe(t_mini *shell, t_token *token, int pipe_status)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (pipe_status == 1)
		{
			dup2(shell->pipin, STDIN);
			close(shell->pipin);
		}
		exec_cmd(shell, token);
		exit(shell->ret);
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		return ;
	}
	if (shell->saved_stdout != -1)
	{
		dup2(shell->saved_stdout, STDOUT);
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
	if (shell->pipin != STDIN)
	{
		close(shell->pipin);
		shell->pipin = STDIN;
	}
	if (pipe_status != 1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->ret = WEXITSTATUS(status);
	}
}

void	handle_redir_exec(t_mini *shell, t_token *token)
{
	int		stdout_copy;
	t_token	*temp;

	if (token->type != CMD)
		return ;
	if (shell->saved_stdout == -1)
		shell->saved_stdout = dup(STDOUT);
	stdout_copy = dup(STDOUT);
	if (stdout_copy == -1)
	{
		perror("minishell: dup");
		return ;
	}
	temp = token;
	while (temp && temp->type != PIPE && temp->type != END)
	{
		if (temp->type == INPUT)
			input_redirection(shell, temp->next);
		else if (temp->type == TRUNC || temp->type == APPEND)
			redir(shell, temp->next, temp->type);
		temp = temp->next;
	}
	if (shell->no_exec == 0 && token->str[0] != '\0')
	{
		exec_cmd(shell, token);
	}
	if (shell->saved_stdout != -1)
	{
		dup2(shell->saved_stdout, STDOUT);
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
	dup2(stdout_copy, STDOUT);
	close(stdout_copy);
}

void	handle_child_process(t_mini *shell)
{
	if (shell->pipin != STDIN)
	{
		dup2(shell->pipin, STDIN);
		close(shell->pipin);
		shell->pipin = STDIN;
	}
	if (shell->saved_stdout != -1)
	{
		dup2(shell->saved_stdout, STDOUT);
		close(shell->saved_stdout);
		shell->saved_stdout = -1;
	}
}

void	exec_pipeline(t_mini *shell)
{
	t_token	*token;
	int		pipe_fd[2];

	token = shell->start;
	while (token && !shell->exit && !shell->no_exec)
	{
		if (token->type == PIPE)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("minishell: pipe");
				return ;
			}
			shell->pipout = pipe_fd[1];
			execute_with_pipe(shell, token->prev, 1);
			shell->pipin = pipe_fd[0];
		}
		token = token->next;
	}
	if (shell->start && !shell->exit && !shell->no_exec)
		execute_with_pipe(shell, shell->start, 0);
}


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
	if (shell->start)
	{
		free_tokens(shell->start);
		shell->start = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->secret_env)
	{
		free_env(shell->secret_env);
		shell->secret_env = NULL;
	}
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

	(void)argc;
	(void)argv;
	init_shell(&shell);
	if (setup_env(&shell, env) != 0)
		return (1);
	input_loop(&shell);
	return (shell.ret);
}
