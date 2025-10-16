/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utilities2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 14:05:49 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/16 14:06:40 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (temp)
		return (temp);
	return (ft_strdup(input));
}

void	process_input(t_mini *mini, char *input)
{
	if (!input || !*input)
		return ;
	if (process_line(mini, input, ft_strlen(input)) != 0)
	{
		mini->ret = 1;
		return ;
	}
}
