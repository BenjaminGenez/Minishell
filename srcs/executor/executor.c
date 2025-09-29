/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:46:11 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:46:11 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

int execute_builtin(t_shell *shell, t_cmd *cmd)
{
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        return (ft_echo(cmd));
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
        return (ft_cd(shell, cmd));
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
        return (ft_pwd());
    else if (ft_strcmp(cmd->args[0], "export") == 0)
        return (ft_export(shell, cmd));
    else if (ft_strcmp(cmd->args[0], "unset") == 0)
        return (ft_unset(shell, cmd));
    else if (ft_strcmp(cmd->args[0], "env") == 0)
        return (ft_env(shell));
    else if (ft_strcmp(cmd->args[0], "exit") == 0)
        ft_exit(shell, cmd);
    return (0);
}

static char *find_command_path(char *cmd, t_shell *shell)
{
    // char *path;
    char *path_env;
    char **paths;
    int i;
    char *full_path;

    if (!cmd || !*cmd)
        return (NULL);

    // Si el comando ya es una ruta absoluta o relativa
    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));

    // Obtener la variable PATH
    path_env = get_env_value(shell->env, "PATH");
    if (!path_env)
        return (NULL);

    // Dividir los directorios del PATH
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);

    // Buscar el comando en cada directorio del PATH
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (!full_path)
        {
            ft_free_array(paths);
            return (NULL);
        }

        if (access(full_path, X_OK) == 0)
        {
            ft_free_array(paths);
            return (full_path);
        }

        free(full_path);
        i++;
    }

    ft_free_array(paths);
    return (NULL);
}

static int execute_single_command(t_shell *shell, t_cmd *cmd)
{
    char *cmd_path;
    char **env_array;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (1);

    // Redirigir entrada/salida si es necesario
    if (cmd->in_fd != STDIN_FILENO)
    {
        if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
            return (1);
        close(cmd->in_fd);
    }

    if (cmd->out_fd != STDOUT_FILENO)
    {
        if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
            return (1);
        close(cmd->out_fd);
    }

    // Ejecutar comando builtin
    if (is_builtin(cmd->args[0]))
        return (execute_builtin(shell, cmd));

    // Ejecutar comando externo
    cmd_path = find_command_path(cmd->args[0], shell);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        return (127);
    }

    // Convertir la lista de entorno a array para execve
    env_array = env_to_array(shell->env);
    if (!env_array)
    {
        free(cmd_path);
        return (1);
    }

    // Configurar señales para el proceso hijo
    setup_child_signals();

    // Ejecutar el comando
    execve(cmd_path, cmd->args, env_array);

    // Si llegamos aquí, hubo un error
    perror("minishell");
    free(cmd_path);
    ft_free_array(env_array);
    return (126);
}

static int execute_pipeline(t_shell *shell, t_cmd *cmds)
{
    int pipe_fd[2];
    int status;
    pid_t pid;
    t_cmd *current;
    int prev_pipe_in = STDIN_FILENO;

    current = cmds;
    while (current)
    {
        if (current->next)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("minishell: pipe");
                return (1);
            }
            current->out_fd = pipe_fd[1];
        }

        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            return (1);
        }

        if (pid == 0)
        {
            // Proceso hijo
            if (prev_pipe_in != STDIN_FILENO)
            {
                dup2(prev_pipe_in, STDIN_FILENO);
                close(prev_pipe_in);
            }

            if (current->out_fd != STDOUT_FILENO)
            {
                dup2(current->out_fd, STDOUT_FILENO);
                close(current->out_fd);
            }

            // Cerrar todos los file descriptors de los pipes
            close(pipe_fd[0]);
            if (current->out_fd != STDOUT_FILENO)
                close(pipe_fd[1]);

            exit(execute_single_command(shell, current));
        }

        // Cerrar los file descriptors que ya no se necesitan
        if (prev_pipe_in != STDIN_FILENO)
            close(prev_pipe_in);
        if (current->out_fd != STDOUT_FILENO)
            close(current->out_fd);

        prev_pipe_in = pipe_fd[0];
        current = current->next;
    }

    // Esperar a que terminen todos los procesos hijos
    pid_t child_pid;
    int exit_status = 0;

    while ((child_pid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
            exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGINT)
            {
                exit_status = 130; // 128 + SIGINT
                write(STDOUT_FILENO, "\n", 1);
            }
            else if (WTERMSIG(status) == SIGQUIT)
            {
                exit_status = 131; // 128 + SIGQUIT
                write(STDOUT_FILENO, "Quit: 3\n", 9);
            }
            else
                exit_status = 128 + WTERMSIG(status);
        }
    }

    // Restaurar el manejador de señales del shell
    init_signals();

    return (exit_status);
}

int execute(t_shell *shell)
{
    if (!shell || !shell->cmds)
        return (1);

    // Si solo hay un comando y es un builtin, ejecutarlo en el proceso actual
    if (!shell->cmds->next && is_builtin(shell->cmds->args[0]))
        return (execute_builtin(shell, shell->cmds));

    // En caso contrario, usar fork y ejecutar en un proceso hijo
    return (execute_pipeline(shell, shell->cmds));
}
