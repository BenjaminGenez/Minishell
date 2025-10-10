#include "minishell.h"
char **command_completion(const char *text, int start, int end)
{
    (void)end;
    if (start == 0 || rl_line_buffer[start - 1] == ' ')
        return rl_completion_matches(text, rl_filename_completion_function);
    return NULL;
}
char *read_input(void)
{
    char *input;
    g_sig.sigint = 0;
    rl_catch_signals = 0;
    rl_clear_signals();
    rl_clear_pending_input();
    rl_attempted_completion_function = command_completion;
    char *prompt = "minishell> ";
    if (isatty(STDOUT_FILENO) || isatty(STDERR_FILENO))
    {
        write(STDOUT_FILENO, "", 0);
        write(STDERR_FILENO, "", 0);
    }
    input = readline(prompt);
    if (!input)
    {
        if (isatty(STDIN_FILENO))
            write(STDERR_FILENO, "exit\n", 5);
        return (NULL);
    }
    if (*input)
    {
        add_history(input);
    }
    return (input);
}
