#include "minishell.h"

typedef struct	s_history_ctx
{
    struct termios 	original_termios;
    t_history		*history;
    t_history		*current_hist;
}             t_history_ctx;
static t_history_ctx *get_history_ctx(t_mini *mini)
{
    static t_history_ctx ctx = {0};
    (void)mini; 
    return (&ctx);
}
void init_terminal(t_mini *mini)
{
    t_history_ctx   *ctx;
    struct termios  new_termios;
    ctx = get_history_ctx(mini);
    tcgetattr(STDIN_FILENO, &ctx->original_termios);
    new_termios = ctx->original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
void reset_terminal(t_mini *mini)
{
    t_history_ctx *ctx = get_history_ctx(mini);
    tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_termios);
}
void add_to_history(t_mini *mini, const char *command)
{
    (void)mini; 
    if (!command || !*command)
        return;
    add_history(command);
}
void free_history(t_mini *mini)
{
    (void)mini; 
}
char *read_line_with_history(t_mini *mini)
{
    (void)mini; 
    char *line = readline("minishell> ");
    if (!line)
    {
        if (isatty(STDIN_FILENO))
            write(STDERR_FILENO, "exit\n", 5);
        return (NULL);
    }
    if (*line)
        add_history(line);
    return (line);
}
