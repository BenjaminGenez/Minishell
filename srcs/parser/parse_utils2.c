#include "minishell.h"

void	handle_quotes_and_spaces(char *line, char *new, int *i, int *j)
{
	int	sep_type;

	if (!line || !new || !i || !j)
		return ;
	sep_type = is_sep(line, *i);
	if (sep_type == 2)
		handle_heredoc_sep(line, new, i, j);
	else if (sep_type == 1)
		handle_other_sep(line, new, i, j);
	else if (is_whitespace(line[*i]))
		handle_whitespace_seq(line, new, i, j);
	else if (line[*i] == '\\' && line[(*i) + 1])
		handle_escaped_char(line, new, i, j);
	else if (line[*i] == '"' || line[*i] == '\'')
		handle_quoted_str(line, new, i, j);
	else
		new[(*j)++] = line[(*i)++];
}
