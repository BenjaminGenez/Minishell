/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:48:56 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_sigint_received = 0;
volatile sig_atomic_t g_sigquit_received = 0;

static void handle_sigint(int sig)
{
    (void)sig;
    g_sigint_received = 1;
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

static void handle_sigquit(int sig)
{
    (void)sig;
    g_sigquit_received = 1;
    rl_on_new_line();
    rl_redisplay();
}

static void handle_sigint_child(int sig)
{
    (void)sig;
    g_sigint_received = 1;
    write(STDOUT_FILENO, "\n", 1);
}

static void handle_sigquit_child(int sig)
{
    (void)sig;
    g_sigquit_received = 1;
    write(STDOUT_FILENO, "Quit: 3\n", 9);
}

void    init_signals(void)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;
    
    // Configurar manejador para SIGINT (Ctrl+C)
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    
    // Configurar manejador para SIGQUIT (Ctrl+\)
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &sa_quit, NULL);
    
    // Ignorar SIGQUIT en el shell principal
    signal(SIGQUIT, SIG_IGN);
    
    // Inicializar variables globales
    g_sigint_received = 0;
    g_sigquit_received = 0;
}

void    setup_child_signals(void)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;
    
    // Configurar manejador para SIGINT en procesos hijos
    sa_int.sa_handler = handle_sigint_child;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    
    // Configurar manejador para SIGQUIT en procesos hijos
    sa_quit.sa_handler = handle_sigquit_child;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void    ignore_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void    reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

int     was_sigint_received(void)
{
    return (g_sigint_received);
}

int     was_sigquit_received(void)
{
    return (g_sigquit_received);
}

void    reset_signal_flags(void)
{
    g_sigint_received = 0;
    g_sigquit_received = 0;
}
