#include "minishell.h"

int	ft_export(char **args, t_mini *mini)
{
	int	i;
	int	error;

	error = 0;
	if (!args[1])
		display_sorted_env(mini->env);
	else
	{
		i = 1;
		while (args[i])
		{
			error = process_export_arg(args[i], mini->env, mini->secret_env);
			i++;
		}
	}
	return (error);
}
