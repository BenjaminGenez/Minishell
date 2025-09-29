/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:38:07 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_shell(t_shell *shell, char **envp)
{
    shell->env = env_to_list(envp);
    shell->cmds = NULL;
    shell->exit_status = 0;
    shell->is_interactive = isatty(STDIN_FILENO);
    shell->is_running = 1;

    // Inicializar señales
    init_signals();

    // Configurar el manejador para SIGINT en el shell principal
    signal(SIGINT, SIG_IGN);

    // Configurar el manejador para SIGQUIT para que sea ignorado en el shell principal
    signal(SIGQUIT, SIG_IGN);
}

void cleanup_shell(t_shell *shell)
{
    if (shell->env)
        free_env(&shell->env);
    if (shell->cmds)
        free_cmds(&shell->cmds);
    rl_clear_history();
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char *input;

    (void)argc;
    (void)argv;

    init_shell(&shell, envp);

    while (shell.is_running)
    {
        // Restablecer las banderas de señal
        reset_signal_flags();

        // Mostrar el prompt y leer la entrada
        input = readline(get_prompt());

        // Si se recibió SIGINT, limpiar y continuar
        if (was_sigint_received())
        {
            shell.exit_status = 130; // 128 + SIGINT
            if (input)
                free(input);
            continue;
        }

        // Si se presionó Ctrl+D (EOF)
        if (!input)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            break;
        }

        // Si la entrada no está vacía
        if (*input)
        {
            add_history(input);
            if (parse_input(&shell, input) == 0)
                shell.exit_status = execute(&shell);
            free_cmds(&shell.cmds);
        }

        free(input);

        // Si se recibió SIGQUIT, ignorarlo en el shell principal
        if (was_sigquit_received())
        {
            shell.exit_status = 131; // 128 + SIGQUIT
            ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
        }
    }

    cleanup_shell(&shell);
    return (shell.exit_status);
}
