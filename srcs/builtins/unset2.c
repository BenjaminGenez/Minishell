#include "minishell.h"

void	remove_env_node(t_mini *mini, t_env *node_to_remove)
{
	if (mini->env == node_to_remove && node_to_remove->next == NULL)
	{
		free(mini->env->value);
		mini->env->value = NULL;
		mini->env->next = NULL;
		return ;
	}
	free(node_to_remove->value);
	free(node_to_remove);
}

int	unset_variable(char *cmd_arg, t_mini *mini)
{
	t_env	*current_env;

	current_env = mini->env;
	if (!current_env)
		return (SUCCESS);
	if (remove_first_node(mini, current_env, cmd_arg))
		return (SUCCESS);
	while (current_env && current_env->next && current_env->next->next)
	{
		if (remove_middle_node(mini, current_env, cmd_arg))
			return (SUCCESS);
		current_env = current_env->next;
	}
	if (current_env && current_env->next && ft_strncmp(cmd_arg,
			current_env->next->value,
			get_var_name_len(current_env->next->value)) == 0)
	{
		remove_env_node(mini, current_env->next);
		current_env->next = NULL;
	}
	return (SUCCESS);
}

int	ft_unset(char **args, t_mini *mini)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	i = 1;
	status = 0;
	while (args[i])
	{
		if (!validate_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			unset_variable(args[i], mini);
		i++;
	}
	return (status);
}
