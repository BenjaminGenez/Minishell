/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/10/04 00:27:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig g_sig;

void handle_redir_exec(t_mini *shell, t_token *token)
{
    t_token *prev_sep_token;
    t_token *next_sep_token;
    int pipe_status;

    prev_sep_token = prev_sep(token, NOSKIP);
    next_sep_token = next_sep(token, NOSKIP);
    pipe_status = 0;

    if (is_type(prev_sep_token, TRUNC))
        redir(shell, token, TRUNC);
    else if (is_type(prev_sep_token, APPEND))
        redir(shell, token, APPEND);
    else if (is_type(prev_sep_token, INPUT))
        input(shell, token);
    else if (is_type(prev_sep_token, PIPE))
        pipe_status = minipipe(shell);

    if (next_sep_token && is_type(next_sep_token, END) == 0 && pipe_status != 1)
        handle_redir_exec(shell, next_sep_token->next);
    if ((is_type(prev_sep_token, END) || is_type(prev_sep_token, PIPE) || !prev_sep_token) && pipe_status != 1 && shell->no_exec == 0)
        exec_cmd(shell, token);
}

void exec_pipeline(t_mini *shell)
{
    t_token *cmd;
    int child_status;

    cmd = next_run(shell->start, NOSKIP);
    if (is_types(shell->start, "TAI"))
        cmd = shell->start->next;

    while (shell->exit == 0 && cmd)
    {
        shell->charge = 1;
        shell->parent = 1;
        shell->last = 1;

        handle_redir_exec(shell, cmd);

        reset_std(shell);
        close_fds(shell);
        reset_fds(shell);

        waitpid(-1, &child_status, 0);
        child_status = WEXITSTATUS(child_status);
        if (shell->last == 0)
            shell->ret = child_status;

        if (shell->parent == 0)
        {
            free_token(shell->start);
            exit(shell->ret);
        }

        shell->no_exec = 0;
        cmd = next_run(cmd, SKIP);
    }
}

static void init_shell(t_mini *shell)
{
    ft_bzero(shell, sizeof(t_mini));
    shell->in = dup(STDIN);
    shell->out = dup(STDOUT);
    shell->exit = 0;
    shell->ret = 0;
    shell->no_exec = 0;
    reset_fds(shell);
}

static int setup_env(t_mini *shell, char **env)
{
    if (setup_env_list(shell, env) != 0)
        return (1);
    if (setup_secret_env(shell, env) != 0)
        return (1);
    increment_shell_level(shell->env);
    return (0);
}

static void cleanup_shell(t_mini *shell)
{
    free_env(shell->env);
    free_env(shell->secret_env);
}

int main(int argc, char **argv, char **env)
{
    t_mini shell;

    (void)argc;
    (void)argv;

    init_shell(&shell);

    if (setup_env(&shell, env) != 0)
        return (1);

    while (shell.exit == 0)
    {
        sig_init();
        parse(&shell);

        if (shell.start != NULL && check_line(&shell, shell.start))
            exec_pipeline(&shell);

        free_token(shell.start);
    }

    cleanup_shell(&shell);
    return (shell.ret);
}