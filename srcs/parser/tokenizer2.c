#include "minishell.h"

int	next_alloc(char *line, int *i)
{
	int	count;

	count = 0;
	while (line[*i + count] && is_sep(line, *i + count) == 0)
	{
		if (!process_escaped_sequence(line, i, &count))
			count++;
	}
	return (count);
}
