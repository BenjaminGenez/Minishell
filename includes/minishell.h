/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:38:07 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:38:07 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

// Estructuras de datos
typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_cmd
{
    char **args;
    int in_fd;
    int out_fd;
    struct s_cmd *next;
} t_cmd;

typedef struct s_shell
{
    t_env *env;
    t_cmd *cmds;
    int exit_status;
    int is_interactive;
    int is_running;
} t_shell;

t_env *env_to_list(char **envp);
char *get_env_value(t_env *env, char *key);
int set_env_value(t_env **env, char *key, char *value);
int unset_env(t_env **env, char *key);
char **env_to_array(t_env *env);

// Prototipos de funciones
// Inicialización
void init_shell(t_shell *shell, char **envp);
void init_signals(void);

// Señales
extern volatile sig_atomic_t g_sigint_received;
extern volatile sig_atomic_t g_sigquit_received;

void setup_child_signals(void);
void ignore_signals(void);
void reset_signals(void);
int was_sigint_received(void);
int was_sigquit_received(void);
void reset_signal_flags(void);

// Prompt
char *get_prompt(void);
void read_input(t_shell *shell);

// Parser
int parse_input(t_shell *shell, char *input);
int tokenize_input(char *input, t_cmd **cmds);
int expand_variables(t_shell *shell, t_cmd *cmd);

// Ejecución
int execute(t_shell *shell);
int execute_builtin(t_shell *shell, t_cmd *cmd);
void execute_external(t_shell *shell, t_cmd *cmd);

// Builtins
int ft_echo(t_cmd *cmd);
int ft_cd(t_shell *shell, t_cmd *cmd);
int ft_pwd(void);
int ft_export(t_shell *shell, t_cmd *cmd);
int ft_unset(t_shell *shell, t_cmd *cmd);
int ft_env(t_shell *shell);
int ft_exit(t_shell *shell, t_cmd *cmd);

// Variables de entorno
t_env *env_to_list(char **envp);
char *get_env_value(t_env *env, char *key);
int set_env_value(t_env **env, char *key, char *value);
int unset_env(t_env **env, char *key);
char **env_to_array(t_env *env);

// Redirecciones
int handle_redirections(t_cmd *cmd);
int handle_heredoc(char *delimiter);

// Pipes
int handle_pipes(t_shell *shell, t_cmd *cmds);

// Utils
void free_cmds(t_cmd **cmds);
void free_env(t_env **env);
void handle_error(char *msg, int exit_status);
char *ft_strjoin_free(char *s1, char *s2);
char *ft_strdup_until(const char *str, char c);

#endif
