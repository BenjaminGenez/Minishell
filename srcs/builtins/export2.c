#include "minishell.h"

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

int	add_env_var(const char *var_value, t_env *env_list)
{
	t_env	*new_node;

	if (!var_value || !env_list)
		return (-1);
	if (env_list->value == NULL)
	{
		env_list->value = ft_strdup(var_value);
		return (SUCCESS);
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (-1);
	new_node->value = ft_strdup(var_value);
	new_node->next = NULL;
	while (env_list->next)
		env_list = env_list->next;
	env_list->next = new_node;
	return (SUCCESS);
}

int	update_existing_var(t_env *env_list, char *new_var)
{
	char	new_name[BUFF_SIZE];
	char	existing_name[BUFF_SIZE];

	if (!env_list || !new_var)
		return (SUCCESS);
	export_extract_var_name(new_name, new_var);
	while (env_list)
	{
		if (env_list->value)
		{
			export_extract_var_name(existing_name, env_list->value);
			if (ft_strcmp(new_name, existing_name) == 0)
			{
				free(env_list->value);
				env_list->value = ft_strdup(new_var);
				return (1);
			}
		}
		env_list = env_list->next;
	}
	return (SUCCESS);
}

int	add_to_both_envs(char *cmd_arg, t_env *env_list,
	t_env *secret_env, int validation_result)
{
	if (validation_result == 1)
		add_env_var(cmd_arg, env_list);
	add_env_var(cmd_arg, secret_env);
	return (SUCCESS);
}

int	process_export_arg(char *cmd_arg, t_env *env_list, t_env *secret_env)
{
	int	validation_result;

	if (!cmd_arg || !*cmd_arg)
		return (export_display_error(0, cmd_arg));
	validation_result = export_is_valid_env(cmd_arg);
	if (validation_result <= 0)
		return (export_display_error(validation_result, cmd_arg));
	if (update_existing_var(env_list, cmd_arg) == 1
		|| update_existing_var(secret_env, cmd_arg) == 1)
		return (SUCCESS);
	return (add_to_both_envs(cmd_arg, env_list, secret_env, validation_result));
}
