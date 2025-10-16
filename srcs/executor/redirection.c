/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_output_redir(t_mini *shell, t_token *token, int type)
{
	int	fd;

	if (type == TRUNC)
		fd = open(token->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(token->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell");
		shell->no_exec = 1;
		return ;
	}
	if (shell->out != STDOUT_FILENO)
		close(shell->out);
	shell->out = fd;
}

static void	handle_input_redir(t_mini *shell, t_token *token)
{
	int	fd;

	fd = open(token->next->str, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		shell->no_exec = 1;
		return ;
	}
	if (shell->in != STDIN_FILENO)
		close(shell->in);
	shell->in = fd;
}

static void	handle_heredoc_redir(t_mini *shell)
{
	if (shell->in != STDIN_FILENO)
		close(shell->in);
	shell->in = dup(shell->fdin);
	if (shell->in == -1)
	{
		perror("minishell: dup");
		shell->no_exec = 1;
		return ;
	}
}

void	handle_redirection(t_mini *shell, t_token *token, int type)
{
	if (!token || !token->next || !token->next->str)
		return ;
	if (type == TRUNC || type == APPEND)
		handle_output_redir(shell, token, type);
	else if (type == INPUT)
		handle_input_redir(shell, token);
	else if (type == HEREDOC)
		handle_heredoc_redir(shell);
}

int	handle_path_search(char **path_dirs, char **cmd_args, t_env *env_list)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (path_dirs && path_dirs[i])
	{
		cmd_path = find_cmd_in_dir(path_dirs[i], cmd_args[0]);
		if (cmd_path)
		{
			handle_bin_child_process(cmd_path, cmd_args, env_list);
			free(cmd_path);
			return (0);
		}
		i++;
	}
	display_error_msg(cmd_args[0]);
	return (127);
}
