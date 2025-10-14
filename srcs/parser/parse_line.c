/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:31 by user              #+#    #+#             */
/*   Updated: 2025/10/13 20:45:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================== LOCAL PROTOTYPES ===========================
*/

/*
** Allocates memory for a new string with spaces around special characters.
** Returns the allocated string or NULL on failure.
*/
/*
** Allocates memory for a new string with spaces around special characters.
** 
** @param line The input line to process
** @return char* The allocated string or NULL on failure
*/
char	*space_alloc(char *line)
{
	char	*new;
	int		count;
	int		i;

	if (!line)
		return (NULL);

	count = 0;
	i = 0;
	while (line[i])
	{
		if (is_sep(line, i))
			count++;
		i++;
	}

	new = malloc(sizeof(char) * (i + 2 * count + 1));
	if (!new)
		return (NULL);

	return (new);
}

/*
** Handles special characters, quotes, and spaces in the input line.
** Modifies the new string with proper spacing around special characters.
*/
/*
** Handles special characters, quotes, and spaces in the input line.
** Modifies the new string with proper spacing around special characters.
**
** @param line The input line
** @param new The output buffer
** @param i Pointer to current position in input line
** @param j Pointer to current position in output buffer
*/
void	handle_quotes_and_spaces(char *line, char *new, int *i, int *j)
{
	int	sep_type;

	if (!line || !new || !i || !j)
		return ;

	sep_type = is_sep(line, *i);
	
	// Handle here-document operator <<
	if (sep_type == 2)
	{
		new[(*j)++] = ' ';
		new[(*j)++] = line[(*i)++];
		new[(*j)++] = line[(*i)++];
		new[(*j)++] = ' ';
	}
	// Handle other separators
	else if (sep_type == 1)
	{
		new[(*j)++] = ' ';
		new[(*j)++] = line[(*i)++];
		if (line[*i] == '>')
			new[(*j)++] = line[(*i)++];
		new[(*j)++] = ' ';
	}
	// Handle whitespace
	else if (is_whitespace(line[*i]))
	{
		new[(*j)++] = ' ';
		while (is_whitespace(line[*i]))
			(*i)++;
	}
	// Handle escaped characters
	else if (line[*i] == '\\' && line[(*i) + 1])
	{
		new[(*j)++] = line[++(*i)];
		(*i)++;
	}
	// Handle quotes
	else if (line[*i] == '"' || line[*i] == '\'')
	{
		char quote;

		quote = line[(*i)++];
		new[(*j)++] = quote;
		while (line[*i] && line[*i] != quote)
		{
			if (line[*i] == '\\' && quote == '"' && line[(*i) + 1])
			{
				new[(*j)++] = line[(*i)++];
				new[(*j)++] = line[(*i)++];
			}
			else
				new[(*j)++] = line[(*i)++];
		}
		if (line[*i] == quote)
			new[(*j)++] = line[(*i)++];
	}
	// Regular character
	else
		new[(*j)++] = line[(*i)++];
}

/*
** Processes a line by adding spaces around special characters.
** Frees the original line and returns the new processed line.
*/
/*
** Processes a line by adding spaces around special characters.
** Frees the original line and returns the new processed line.
**
** @param line The line to process
** @return char* The processed line or NULL on error
*/
char	*space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	if (!line)
		return (NULL);

	new = space_alloc(line);
	if (!new)
	{
		free(line);
		return (NULL);
	}

	i = 0;
	j = 0;
	while (line[i])
		handle_quotes_and_spaces(line, new, &i, &j);

	new[j] = '\0';
	free(line);
	return (new);
}

/*
** Checks for unclosed quotes in the input line.
** Returns 1 if there are unclosed quotes, 0 otherwise.
*/
/*
** Checks for unclosed quotes in the input line.
**
** @param mini The main shell structure
** @param line Pointer to the line to check
** @return int 1 if there are unclosed quotes, 0 otherwise
*/
int	quote_check(t_mini *mini, char **line)
{
	int		i;
	char	quote;

	if (!mini || !line || !*line)
		return (1);

	i = 0;
	while ((*line)[i])
	{
		// Handle escaped backslash or quote
		if ((*line)[i] == '\\' && ((*line)[i + 1] == '\\' || 
			(*line)[i + 1] == '"'))
			i += 2;
		// Handle other escape sequences
		else if ((*line)[i] == '\\' && (*line)[i + 1])
			i += 2;
		// Handle quotes
		else if ((*line)[i] == '"' || (*line)[i] == '\'')
		{
			quote = (*line)[i++];
			while ((*line)[i] && (*line)[i] != quote)
				i++;
			if (!(*line)[i])
			{
				ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
				ft_putstr_fd("unexpected end of file\n", STDERR_FILENO);
				mini->ret = 2;
				return (1);
			}
			i++;
		}
		else
			i++;
	}
	return (0);
}

/*
** Cleans the input buffer by handling escape characters.
** Returns the cleaned buffer.
*/
/*
** Cleans the input buffer by handling escape characters.
**
** @param buffer The input buffer to clean
** @param bytes_read Number of bytes in the buffer
** @return char* The cleaned buffer
*/
char	*clean_input(char *buffer, int bytes_read)
{
	int i;

	if (!buffer || bytes_read <= 0)
		return (NULL);

	i = 0;
	while (i < bytes_read)
	{
		// Handle escaped backslash
		if (buffer[i] == '\\' && buffer[i + 1] == '\\')
			i += 2;
		// Handle other escape sequences
		else if (buffer[i] == '\\' && buffer[i + 1])
			i += 2;
		else
			i++;
	}

	buffer[bytes_read] = '\0';
	return (buffer);
}

/*
** Processes tokens in the linked list, setting their types.
**
** @param mini The main shell structure
*/
void	process_tokens(t_mini *mini)
{
	t_token	*token;

	if (!mini || !mini->start)
		return ;

	token = mini->start;
	while (token)
	{
		type_arg(token, 0);
		token = token->next;
	}
}

/*
** Checks if the token list contains any here-document tokens.
** Returns 1 if found, 0 otherwise.
*/
/*
** Main function to process a line of input.
**
** @param mini The main shell structure
** @param buffer The input buffer
** @param bytes_read Number of bytes in the buffer
** @return int 0 on success, 1 on error
*/
int	process_line(t_mini *mini, char *buffer, int bytes_read)
{
	char	*line;

	if (!mini || !buffer || bytes_read <= 0)
		return (1);

	line = clean_input(buffer, bytes_read);
	if (!line)
		return (1);

	// Check for SIGINT
	if (g_sig.sigint == 1)
	{
		mini->ret = g_sig.exit_status;
		mem_free(line);
		return (1);
	}

	// Check for unclosed quotes
	if (quote_check(mini, &line))
	{
		mem_free(line);
		return (1);
	}

	// Process the line
	line = space_line(line);
	if (!line)
		return (1);

	// Handle special case for variable expansion
	if (line[0] == '$')
		line[0] = (char)(-line[0]);

	// Tokenize the input
	mini->start = get_tokens(line);
	mem_free(line);

	if (!mini->start)
		return (1);

	// Process the tokens
	squish_args(mini);
	process_tokens(mini);

	// Handle here-documents if present
	if (contains_heredoc(mini->start))
		process_tokens_heredoc(mini);

	return (0);
}
