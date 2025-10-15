#include "minishell.h"

int	ft_cd(char **args, t_mini *mini)
{
	char	*oldpwd;
	int		status;
	t_env	*env;

	env = mini->env;
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR);
		return (1);
	}
	if (cd_get_oldpwd(&oldpwd) != 0)
		return (1);
	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
		status = go_to_home(env, oldpwd);
	else if (ft_strcmp(args[1], "-") == 0)
		status = go_to_oldpwd(env, oldpwd);
	else
		status = go_to_path(args[1], env, oldpwd);
	if (status == 0)
		mini->exit = 0;
	free(oldpwd);
	return (status);
}
