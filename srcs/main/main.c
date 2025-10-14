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
	shell->in = dup(STDIN);
	shell->out = dup(STDOUT);
	shell->exit = 0;
	shell->ret = 0;
	shell->no_exec = 0;
	shell->saved_stdout = -1;
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
	if (!shell)
		return ;
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
	if (shell->start)
	{
		free_token(shell->start);
		shell->start = NULL;
	}
	reset_fds(shell);
	if (shell->in >= 0 && shell->in != STDIN)
	{
		close(shell->in);
		shell->in = -1;
	}
	if (shell->out >= 0 && shell->out != STDOUT)
	{
		close(shell->out);
		shell->out = -1;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	int 	i;

	printf("=== Minishell starting ===\n");
	printf("Arguments: %d\n", argc);
	i = 0;
	while (i < argc)
	{
		printf("Arg %d: %s\n", i, argv[i]);
		i++;
	}
	init_shell(&shell);
	if (setup_env(&shell, env) != 0)
	{
		printf("Failed to set up environment\n");
		return (1);
	}
	printf("Starting input loop...\n");
	input_loop(&shell);
	cleanup_shell(&shell);
	printf("=== Minishell exiting ===\n");
	return (0);
}
