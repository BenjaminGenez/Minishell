/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:45:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/14 14:45:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_standard_fds(t_mini *shell)
{
	(void)shell;
	dup2(STDIN_FILENO, STDIN_FILENO);
	dup2(STDOUT_FILENO, STDOUT_FILENO);
	dup2(STDERR_FILENO, STDERR_FILENO);
}

static int	is_redir_target(t_token *tmp)
{
	if (tmp->prev && (tmp->prev->type == TRUNC
			|| tmp->prev->type == APPEND || tmp->prev->type == INPUT
			|| tmp->prev->type == HEREDOC))
		return (1);
	return (0);
}

static int	count_cmd_tokens(t_token *token)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = token;
	while (tmp && tmp->type != PIPE && tmp->type != END)
	{
		if (tmp->type == CMD || tmp->type == ARG)
		{
			if (!is_redir_target(tmp))
				count++;
		}
		tmp = tmp->next;
	}
	return (count);
}

static int	fill_args_helper(char **args, t_token *tmp, int *i)
{
	if (tmp->type == CMD || tmp->type == ARG)
	{
		if (!is_redir_target(tmp))
		{
			args[*i] = ft_strdup(tmp->str);
			if (!args[*i])
				return (0);
			(*i)++;
		}
	}
	return (1);
}

char	**token_list_to_array(t_token *token)
{
	int		count;
	char	**args;
	t_token	*tmp;
	int		i;

	count = count_cmd_tokens(token);
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	tmp = token;
	while (i < count && tmp && tmp->type != PIPE && tmp->type != END)
	{
		if (!fill_args_helper(args, tmp, &i))
		{
			ft_free_array(args);
			return (NULL);
		}
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}
