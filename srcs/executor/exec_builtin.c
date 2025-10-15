/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:46:07 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:46:09 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "env", "export", "unset",
		"exit", NULL};
	int			idx;

	if (!cmd_name)
		return (0);
	idx = 0;
	while (builtins[idx])
	{
		if (ft_strcmp(cmd_name, builtins[idx]) == 0)
			return (1);
		idx++;
	}
	return (0);
}

int	exec_builtin(char **cmd_args, t_mini *shell)
{
	int		exit_code;
	char	*cmd;

	if (!cmd_args || !cmd_args[0])
		return (1);
	cmd = cmd_args[0];
	exit_code = 0;
	if (ft_strcmp(cmd, "echo") == 0)
		exit_code = ft_echo(cmd_args);
	else if (ft_strcmp(cmd, "cd") == 0)
		exit_code = ft_cd(cmd_args, shell);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = ft_pwd();
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = ft_env(shell->env);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_code = ft_export(cmd_args, shell);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_code = ft_unset(cmd_args, shell);
	else if (ft_strcmp(cmd, "exit") == 0)
		exit_code = ft_exit(cmd_args, shell);
	else
		exit_code = 1;
	return (exit_code);
}

static void	open_and_dup_output(t_mini *mini, t_token *token, int type)
{
	if (type == TRUNC)
		mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		mini->fdout = open(token->str, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (mini->fdout == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(token->str, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	dup2(mini->fdout, STDOUT_FILENO);
}

void	redir(t_mini *mini, t_token *token, int type)
{
	if (type == HEREDOC && mini->fdin != STDIN_FILENO)
	{
		dup2(mini->fdin, STDIN_FILENO);
		close(mini->fdin);
		mini->fdin = STDIN_FILENO;
	}
	else if (type == TRUNC || type == APPEND)
	{
		if (mini->fdout != STDOUT_FILENO)
		{
			close(mini->fdout);
			mini->fdout = STDOUT_FILENO;
		}
		open_and_dup_output(mini, token, type);
	}
	else if (type == INPUT)
		input_redirection(mini, token);
}

void	input_redirection(t_mini *mini, t_token *token)
{
	if (mini->fdin != STDIN_FILENO)
	{
		close(mini->fdin);
		mini->fdin = STDIN_FILENO;
	}
	mini->fdin = open(token->str, O_RDONLY);
	if (mini->fdin == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(token->str, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		mini->ret = 1;
		mini->no_exec = 1;
		return ;
	}
	dup2(mini->fdin, STDIN_FILENO);
}
