/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 23:30:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 23:30:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Forward declarations */
static int	is_valid_fd(int fd);
static void	execute_command(t_mini *shell, char **cmd_args);
static void	expand_command_args(char **cmd_args, t_mini *shell);
static int	process_redirections(t_mini *shell, t_token *token);

static int	is_valid_fd(int fd)
{
	return (fd >= 0 && fd < 1024);
}

static void	execute_command(t_mini *shell, char **cmd_args)
{
	int	saved_stdout;
	int	saved_stdin;

	if (!cmd_args || !cmd_args[0])
		return ;
	if (ft_strcmp(cmd_args[0], "exit") == 0)
	{
		shell->ret = ft_exit(cmd_args, shell);
		return ;
	}
	else if (is_builtin(cmd_args[0]))
	{
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdout == -1)
		{
			perror("minishell: dup");
			shell->ret = 1;
			return ;
		}
		saved_stdin = dup(STDIN_FILENO);
		if (saved_stdin == -1)
		{
			close(saved_stdout);
			perror("minishell: dup");
			shell->ret = 1;
			return ;
		}
		if (is_valid_fd(shell->out) && shell->out != STDOUT_FILENO)
		{
			if (dup2(shell->out, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2");
				close(saved_stdout);
				close(saved_stdin);
				shell->ret = 1;
				return ;
			}
			close(shell->out);
			shell->out = STDOUT_FILENO;
		}
		if (is_valid_fd(shell->in) && shell->in != STDIN_FILENO)
		{
			if (dup2(shell->in, STDIN_FILENO) == -1)
			{
				perror("minishell: dup2");
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdout);
				close(saved_stdin);
				shell->ret = 1;
				return ;
			}
			close(shell->in);
			shell->in = STDIN_FILENO;
		}
		shell->ret = exec_builtin(cmd_args, shell);
		if (dup2(saved_stdout, STDOUT_FILENO) == -1 || 
			dup2(saved_stdin, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			shell->ret = 1;
		}
		close(saved_stdout);
		close(saved_stdin);
	}
	else
	{
		exec_bin(cmd_args, shell);
	}
}

static void	expand_command_args(char **cmd_args, t_mini *shell)
{
	int		idx;
	char	*expanded;

	if (!cmd_args)
		return ;
	idx = 0;
	while (cmd_args[idx])
	{
		expanded = cmd_args[idx];
		if (ft_strchr(expanded, '$') || ft_strchr(expanded, '"') || 
			ft_strchr(expanded, '\''))
		{
			expanded = handle_expansions(expanded, shell->env, shell->ret);
			if (expanded != cmd_args[idx])
			{
				free(cmd_args[idx]);
				cmd_args[idx] = expanded;
			}
		}
		idx++;
	}
}

static int	process_redirections(t_mini *shell, t_token *token)
{
	while (token && token->type != END)
	{
		if (token->type == TRUNC || token->type == APPEND ||
			token->type == INPUT || token->type == HEREDOC)
		{
			handle_redirection(shell, token, token->type);
			if (shell->no_exec == 1)
				return (0);
			token = token->next;
		}
		token = token->next;
	}
	return (1);
}

void	exec_cmd(t_mini *shell, t_token *token)
{
	char	**cmd_args;
	t_token	*start_token;

	start_token = token;
	if (shell->charge == 0)
		return ;
	if (!process_redirections(shell, token))
	{
		reset_standard_fds(shell);
		return ;
	}
	cmd_args = token_list_to_array(start_token);
	if (!cmd_args)
	{
		reset_standard_fds(shell);
		return ;
	}
	expand_command_args(cmd_args, shell);
	if (cmd_args && cmd_args[0])
	{
		if (ft_strcmp(cmd_args[0], "cd") == 0)
			shell->ret = ft_cd(cmd_args, shell);
		else
			execute_command(shell, cmd_args);
	}
	free_array(cmd_args);
	reset_standard_fds(shell);
}
