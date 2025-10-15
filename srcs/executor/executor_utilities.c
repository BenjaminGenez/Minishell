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

// to be fixed! too many functions :(
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

static char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			i++;
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static char	*mark_expansions(char *input)
{
	char	*temp;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	temp = ft_strdup(input);
	if (!temp)
		return (ft_strdup(input));
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (temp[i])
	{
		if (temp[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (temp[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (temp[i] == '$' && !in_single_quote)
			temp[i] = EXPANSION;
		i++;
	}
	return (temp);
}

char	*handle_expansions(char *input, t_env *env, int ret)
{
	char	*result;
	char	*temp;

	if (!input)
		return (NULL);
	temp = mark_expansions(input);
	result = expansions(temp, env, ret);
	free(temp);
	if (!result)
		return (ft_strdup(input));
	temp = remove_quotes(result);
	free(result);
	return (temp ? temp : ft_strdup(input));
}

void	process_input(t_mini *mini, char *input)
{
	if (!input || !*input)
		return ;
	mini->ret = 0;
	if (process_line(mini, input, ft_strlen(input)) != 0)
	{
		mini->ret = 1;
		return ;
	}
}
