/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:02:15 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 19:15:53 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define EXIT_COMMAND 2
#define PROMPT "minishell> "

int execute_builtin(t_mini *shell, char **args, t_token *token)
{
    (void)token;  // Unused parameter
    if (!args || !args[0])
        return (0);
        
    if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(args, shell));
    else if (ft_strcmp(args[0], "pwd") == 0)
        return (ft_pwd());
    else if (ft_strcmp(args[0], "echo") == 0)
        return (ft_echo(args));
    else if (ft_strcmp(args[0], "env") == 0)
        return (ft_env(shell->env));
    else if (ft_strcmp(args[0], "export") == 0)
        return (ft_export(args, shell));
    else if (ft_strcmp(args[0], "unset") == 0)
        return (ft_unset(args, shell));
    else if (ft_strcmp(args[0], "exit") == 0)
        return (ft_exit(args, shell));
    return (0);
}

char **token_to_args(t_token *token)
{
    int     count = 0;
    t_token *tmp = token;
    char    **args;
    int     i;

    while (tmp && tmp->type != PIPE && tmp->type != END)
    {
        if (tmp->type == ARG || tmp->type == CMD)
            count++;
        tmp = tmp->next;
    }

    args = (char **)malloc(sizeof(char *) * (count + 1));
    if (!args)
        return (NULL);

    i = 0;
    tmp = token;
    while (tmp && tmp->type != PIPE && tmp->type != END)
    {
        if (tmp->type == ARG || tmp->type == CMD)
            args[i++] = ft_strdup(tmp->str);
        tmp = tmp->next;
    }
    args[i] = NULL;
    return (args);
}

void free_args(char **args)
{
    int i = 0;
    if (!args)
        return;
    while (args[i])
        free(args[i++]);
    free(args);
}

// Placeholder for here-document functionality
int handle_heredoc_input(t_mini *mini, t_token *tokens)
{
    (void)mini;
    (void)tokens;
    return (0);
}

char *process_heredoc(char *input)
{
    char *heredoc_pos;
    char *delimiter_start;
    char *delimiter_end;
    char *delimiter;
    char *content;
    char *new_input;
    char *before_heredoc;
    char *after_heredoc;
    size_t before_len;

    heredoc_pos = ft_strnstr(input, "<<", ft_strlen(input));
    if (!heredoc_pos)
        return (input);

    // Get the part before the here-document
    before_len = heredoc_pos - input;
    before_heredoc = ft_strndup(input, before_len);
    if (!before_heredoc)
        return (input);

    // Find the delimiter
    delimiter_start = heredoc_pos + 2; // Skip "<<"
    while (*delimiter_start && ft_isspace(*delimiter_start))
        delimiter_start++;
    
    if (!*delimiter_start)
    {
        free(before_heredoc);
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR);
        return (input);
    }

    // Find the end of the delimiter
    delimiter_end = delimiter_start;
    while (*delimiter_end && !ft_isspace(*delimiter_end) && 
           *delimiter_end != '|' && *delimiter_end != ';' && 
           *delimiter_end != '&' && *delimiter_end != '<' && 
           *delimiter_end != '>')
        delimiter_end++;

    // Extract the delimiter
    delimiter = ft_strndup(delimiter_start, delimiter_end - delimiter_start);
    if (!delimiter)
    {
        free(before_heredoc);
        perror("minishell: malloc");
        return (input);
    }
    content = NULL;
    free(delimiter);
    
    if (!content)
    {
        free(before_heredoc);
        return (input);
    }

    // Get the part after the delimiter
    after_heredoc = ft_strdup(delimiter_end);
    if (!after_heredoc)
    {
        free(before_heredoc);
        free(content);
        return (input);
    }

    // Combine everything
    new_input = ft_strjoin(before_heredoc, content);
    free(before_heredoc);
    free(content);
    
    if (!new_input)
    {
        free(after_heredoc);
        return (input);
    }
    
    char *temp = ft_strjoin(new_input, after_heredoc);
    free(new_input);
    free(after_heredoc);
    
    if (!temp)
        return (input);

    free(input);
    return (temp);
}

void	handle_input(t_mini *shell, char *input)
{
	t_token	*current;
	char	**args;
	int		status;

	if (!input || !*input)
		return ;
	process_input(shell, input);
	current = shell->start;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			if (handle_heredoc(shell, current) != 0)
			{
				free_tokens(shell->start);
				shell->start = NULL;
				return ;
			}
		}
		current = current->next;
	}
	if (shell->start && shell->start->type != END)
	{
		if (shell->start->type == CMD && shell->start->str &&
			is_builtin(shell->start->str))
		{
			args = token_to_args(shell->start);
			if (args)
			{
				status = execute_builtin(shell, args, NULL);
				free_args(args);
				if (status == EXIT_COMMAND)
					shell->exit = 1;
			}
		}
		else
			exec_cmd(shell, shell->start);
	}
	if (shell->start)
	{
		free_tokens(shell->start);
		shell->start = NULL;
	}
}

void	input_loop(struct s_mini *shell)
{
	char	*input;

	while (!shell->exit)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*input)
			handle_input(shell, input);
		else
			free(input);  /* Free empty input */
	}
}
