#include "minishell.h"
#include <signal.h>
#include <sys/types.h>

#define EXIT_COMMAND 2

static int execute_builtin(t_mini *shell, char **args)
{
    if (ft_strcmp(args[0], "cd") == 0)
        return (ft_cd(shell, args));
    else if (ft_strcmp(args[0], "pwd") == 0)
        return (ft_pwd());
    else if (ft_strcmp(args[0], "echo") == 0)
        return (ft_echo(args));
    else if (ft_strcmp(args[0], "env") == 0)
        return (ft_env(shell));
    else if (ft_strcmp(args[0], "export") == 0)
        return (ft_export(shell, args));
    else if (ft_strcmp(args[0], "unset") == 0)
        return (ft_unset(shell, args));
    else if (ft_strcmp(args[0], "exit") == 0)
        return (mini_exit(shell, args));
    return (0);
}

static char **token_to_args(t_token *token)
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

static void free_args(char **args)
{
    int i = 0;
    if (!args)
        return;
    while (args[i])
        free(args[i++]);
    free(args);
}

void handle_input(t_mini *shell, char *input)
{
    char    **args;
    int     saved_stdout;
    int     saved_stdin;
    int     status = 0;

    if (!input || !*input)
        return;

    add_history(input);
    shell->start = get_tokens(input);
    if (!shell->start)
        return;

    // Type the arguments
    t_token *current = shell->start;
    while (current)
    {
        type_arg(current, 0);
        current = current->next;
    }

    // Check if the command line is valid
    if (!check_line(shell, shell->start))
    {
        free_token(shell->start);
        shell->start = NULL;
        return;
    }

    // Save original file descriptors
    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);

    // Execute the command
    args = token_to_args(shell->start);
    if (args && args[0])
    {
        if (is_builtin(args[0]))
        {
            status = execute_builtin(shell, args);
            // If the command was exit, exit the shell immediately
            if (status == EXIT_COMMAND || shell->exit)
            {
                free_args(args);
                free_token(shell->start);
                shell->start = NULL;
                return;
            }
        }
        else
        {
            exec_pipeline(shell);
            status = 0;  // Default status for non-builtin commands
        }
    }

    // Restore file descriptors
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);

    // Cleanup
    free_args(args);
    free_token(shell->start);
    shell->start = NULL;

    // If the command was exit, exit the shell
    if (shell->exit)
        exit(shell->ret);
}

void setup_signal_handlers(int interactive)
{
    // Initialize signal handler variables
    sig_init();
    
    if (interactive)
    {
        // Set up signal handlers for interactive mode
        struct sigaction sa;
        
        // Set up SIGINT handler
        sa.sa_handler = sig_int;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
        
        // Ignore SIGQUIT in interactive mode
        signal(SIGQUIT, SIG_IGN);
        
        // Configure readline signal handling
        rl_catch_signals = 0;
        rl_clear_signals();
        rl_set_signals();
    }
    else
    {
        // Set default signal handlers for non-interactive mode
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
}
void input_loop(struct s_mini *shell)
{
    char *input;
    
    // Initialize the terminal
    setup_terminal();
    setup_signal_handlers(1);
    
    // Initialize readline
    using_history();
    rl_bind_key('\t', rl_insert);  // Disable tab completion
    
    while (1)
    {
        // Only display prompt if we're in an interactive terminal
        if (isatty(STDIN_FILENO))
            input = readline("minishell> ");
        else
            input = readline("");  // No prompt for non-interactive mode
        
        // Handle EOF (Ctrl+D)
        if (!input)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        
        // Skip empty input
        if (!*input)
        {
            free(input);
            continue;
        }
        
        // Add to history if not empty
        add_history(input);
        
        // Handle the input
        handle_input(shell, input);
        
        // Free the input after processing
        free(input);
        
        // Check if we should exit the shell
        if (shell->exit)
        {
            break;
        }
    }
    
    // Cleanup and exit
    if (shell->env)
        free_env_list(shell->env);
    if (shell->secret_env)
        free_env_list(shell->secret_env);
    exit(shell->ret);
}
