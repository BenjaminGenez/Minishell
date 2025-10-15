/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:47:20 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:55:07 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	varlcpy(char *new_arg, const char *env_value, int pos)
{
	int	i;

	i = 0;
	while (env_value[i])
		new_arg[pos++] = env_value[i++];
	return (i);
}

void	handle_var_increment(char *arg, int *j)
{
	if (arg[*j] == '?')
		(*j)++;
	if (ft_isdigit(arg[*j]) == 0 && arg[*j - 1] != '?')
	{
		while (is_env_char(arg[*j]) == 1)
			(*j)++;
	}
	else
	{
		if (arg[*j - 1] != '?')
			(*j)++;
	}
}

void	insert_var(t_expansions *ex, char *arg, t_env *env, int ret)
{
	char	*env_value;

	env_value = get_var_value(arg, ex->j, env, ret);
	if (env_value)
		ex->i += varlcpy(ex->new_arg, env_value, ex->i);
	mem_free(env_value);
	handle_var_increment(arg, &ex->j);
}

void	process_expansion(t_expansions *ex, char *arg, t_env *env, int ret)
{
	while (arg[ex->j] == EXPANSION)
	{
		ex->j++;
		if ((arg[ex->j] == '\0' || ft_isalnum(arg[ex->j]) == 0)
			&& arg[ex->j] != '?')
			ex->new_arg[ex->i++] = '$';
		else
			insert_var(ex, arg, env, ret);
	}
	ex->new_arg[ex->i++] = arg[ex->j++];
}

char	*expansions(char *arg, t_env *env, int ret)
{
	t_expansions	ex;
	int				new_arg_len;

	new_arg_len = arg_alloc_len(arg, env, ret);
	ex.new_arg = malloc(sizeof(char) * new_arg_len + 1);
	if (!ex.new_arg)
		return (NULL);
	ex.i = 0;
	ex.j = 0;
	while (ex.i < new_arg_len && arg[ex.j])
		process_expansion(&ex, arg, env, ret);
	ex.new_arg[ex.i] = '\0';
	return (ex.new_arg);
}
