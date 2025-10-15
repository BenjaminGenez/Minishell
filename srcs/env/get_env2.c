#include "minishell.h"

static char	*extract_var_name(char *dest_buffer, const char *env_string)
{
	int	pos;

	pos = 0;
	while (env_string[pos] && env_string[pos] != '='
		&& ft_strlen(env_string) < BUFF_SIZE)
	{
		dest_buffer[pos] = env_string[pos];
		pos++;
	}
	dest_buffer[pos] = '\0';
	return (dest_buffer);
}

static int	calc_value_len(const char *env_str)
{
	int	pos;
	int	value_len;

	pos = 0;
	value_len = 0;
	while (env_str[pos] && env_str[pos] != '=')
		pos++;
	if (env_str[pos] == '=')
		pos++;
	while (env_str[pos])
	{
		pos++;
		value_len++;
	}
	return (value_len);
}

static char	*extract_env_value(char *env_str)
{
	int		src_pos;
	int		dst_pos;
	int		value_size;
	char	*result;

	value_size = calc_value_len(env_str) + 1;
	result = malloc(sizeof(char) * value_size);
	if (!result)
		return (NULL);
	src_pos = 0;
	while (env_str[src_pos] && env_str[src_pos] != '=')
		src_pos++;
	if (env_str[src_pos] == '=')
		src_pos++;
	dst_pos = 0;
	while (env_str[src_pos])
		result[dst_pos++] = env_str[src_pos++];
	result[dst_pos] = '\0';
	return (result);
}

char	*find_env_value(char *var_name, t_env *env_list)
{
	char	name_buffer[BUFF_SIZE];
	char	*result;

	result = ft_strdup("");
	while (env_list && env_list->value)
	{
		extract_var_name(name_buffer, env_list->value);
		if (ft_strcmp(var_name, name_buffer) == 0)
		{
			free(result);
			result = extract_env_value(env_list->value);
			return (result);
		}
		env_list = env_list->next;
	}
	return (result);
}
