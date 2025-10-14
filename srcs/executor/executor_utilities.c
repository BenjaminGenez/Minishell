/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:45:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/14 14:45:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Resets the standard file descriptors to their original values
 * 
 * @param shell The main shell structure
 */
void reset_standard_fds(t_mini *shell)
{
    (void)shell; // Mark as unused for now
    // TODO: Implement proper file descriptor reset logic
    // This is a temporary implementation
    dup2(STDIN_FILENO, STDIN_FILENO);
    dup2(STDOUT_FILENO, STDOUT_FILENO);
    dup2(STDERR_FILENO, STDERR_FILENO);
}

/**
 * @brief Converts a token list to a string array
 * 
 * @param token The head of the token list
 * @return char** The array of strings, or NULL on failure
 */
char **token_list_to_array(t_token *token)
{
    int     count;
    char    **args;
    t_token *tmp;
    int     i;

    // Count the number of arguments
    count = 0;
    tmp = token;
    while (tmp && tmp->type != PIPE && tmp->type != END)
    {
        if (tmp->str && tmp->str[0])
            count++;
        tmp = tmp->next;
    }

    // Allocate memory for the array
    args = (char **)malloc(sizeof(char *) * (count + 1));
    if (!args)
        return (NULL);

    // Copy the token strings to the array
    i = 0;
    tmp = token;
    while (i < count && tmp && tmp->type != PIPE && tmp->type != END)
    {
        if (tmp->str && tmp->str[0])
        {
            args[i] = ft_strdup(tmp->str);
            if (!args[i])
            {
                ft_free_array(args);
                return (NULL);
            }
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return (args);
}

/**
 * @brief Handles input/output redirection
 * 
 * @param shell The main shell structure (unused for now)
 * @param token The redirection token
 * @param type The type of redirection
 */
void handle_redirection(t_mini *shell, t_token *token, int type)
{
    int fd;
    (void)shell; // Mark as unused for now

    if (!token || !token->next || !token->next->str)
        return;

    if (type == TRUNC) // >
    {
        fd = open(token->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("minishell");
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    else if (type == APPEND) // >>
    {
        fd = open(token->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
        {
            perror("minishell");
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    else if (type == INPUT) // <
    {
        fd = open(token->next->str, O_RDONLY);
        if (fd == -1)
        {
            perror("minishell");
            return;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    // HEREDOC is handled separately in heredoc.c
}

/**
 * @brief Handles environment variable expansion and other expansions
 * 
 * @param input The input string to expand
 * @param env The environment variable list (unused for now)
 * @param ret The exit status of the last command (unused for now)
 * @return char* The expanded string, or NULL on failure
 */
char *handle_expansions(char *input, t_env *env, int ret)
{
    (void)env; // Mark as unused for now
    (void)ret;  // Mark as unused for now
    
    // For now, just return a copy of the input string
    // TODO: Implement proper expansion logic
    return (ft_strdup(input));
}

/**
 * @brief Processes the input line and executes the corresponding command
 * 
 * @param mini The main shell structure
 * @param input The input line to process
 */
void process_input(t_mini *mini, char *input)
{
    if (!input || !*input)
        return;

    // For now, just print the input
    // TODO: Implement proper input processing and command execution
    printf("Processing input: %s\n", input);
    
    // Set a default return value
    mini->ret = 0;
    
    // Process the input
    if (process_line(mini, input, ft_strlen(input)) != 0)
    {
        mini->ret = 1;
        return;
    }
    
    // If there are tokens, execute them
    if (mini->start)
    {
        // TODO: Implement proper command execution
        printf("Executing command...\n");
        free_tokens(mini->start);
        mini->start = NULL;
    }
}
