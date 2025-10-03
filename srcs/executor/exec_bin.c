/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:45:50 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:55:04 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int display_error_msg(char *cmd_path)
{
    DIR *dir_ptr;
    int file_fd;
    int error_code;

    file_fd = open(cmd_path, O_WRONLY);
    dir_ptr = opendir(cmd_path);
    ft_putstr_fd("minishell: ", STDERR);
    ft_putstr_fd(cmd_path, STDERR);
    if (ft_strchr(cmd_path, '/') == NULL)
        ft_putendl_fd(": command not found", STDERR);
    else if (file_fd == -1 && dir_ptr == NULL)
        ft_putendl_fd(": No such file or directory", STDERR);
    else if (file_fd == -1 && dir_ptr != NULL)
        ft_putendl_fd(": is a directory", STDERR);
    else if (file_fd != -1 && dir_ptr == NULL)
        ft_putendl_fd(": Permission denied", STDERR);
    if (ft_strchr(cmd_path, '/') == NULL || (file_fd == -1 && dir_ptr == NULL))
        error_code = UNKNOWN_COMMAND;
    else
        error_code = IS_DIRECTORY;
    if (dir_ptr)
        closedir(dir_ptr);
    ft_close(file_fd);
    return (error_code);
}

int execute_binary(char *bin_path, char **cmd_args, t_env *env_list, t_mini *shell)
{
    char **env_arr;
    char *env_str;
    int exit_code;

    exit_code = SUCCESS;
    g_sig.pid = fork();
    if (g_sig.pid == 0)
    {
        env_str = build_env_str(env_list);
        env_arr = ft_split(env_str, '\n');
        mem_free(env_str);
        if (ft_strchr(bin_path, '/') != NULL)
            execve(bin_path, cmd_args, env_arr);
        exit_code = display_error_msg(bin_path);
        free_tab(env_arr);
        free_token(shell->start);
        exit(exit_code);
    }
    else
        waitpid(g_sig.pid, &exit_code, 0);
    if (g_sig.sigint == 1 || g_sig.sigquit == 1)
        return (g_sig.exit_status);
    if (exit_code == 32256 || exit_code == 32512)
        exit_code = exit_code / 256;
    else
        exit_code = !!exit_code;
    return (exit_code);
}

char *join_path(const char *dir, const char *filename)
{
    char *temp_str;
    char *full_path;

    temp_str = ft_strjoin(dir, "/");
    full_path = ft_strjoin(temp_str, filename);
    mem_free(temp_str);
    return (full_path);
}

char *find_cmd_in_dir(char *dir_path, char *cmd_name)
{
    DIR *directory;
    struct dirent *entry;
    char *result_path;

    result_path = NULL;
    directory = opendir(dir_path);
    if (!directory)
        return (NULL);
    while ((entry = readdir(directory)))
    {
        if (ft_strcmp(entry->d_name, cmd_name) == 0)
            result_path = join_path(dir_path, entry->d_name);
    }
    closedir(directory);
    return (result_path);
}

int exec_bin(char **cmd_args, t_env *env_list, t_mini *shell)
{
    int dir_idx;
    char **path_dirs;
    char *cmd_path;
    int exit_code;
    t_env *env_ptr;

    dir_idx = 0;
    exit_code = UNKNOWN_COMMAND;
    env_ptr = env_list;
    while (env_ptr && env_ptr->value && ft_strncmp(env_ptr->value, "PATH=", 5) != 0)
        env_ptr = env_ptr->next;
    if (env_ptr == NULL || env_ptr->next == NULL)
        return (execute_binary(cmd_args[0], cmd_args, env_list, shell));
    path_dirs = ft_split(env_ptr->value, ':');
    if (!cmd_args[0] && !path_dirs[0])
        return (ERROR);
    dir_idx = 1;
    cmd_path = find_cmd_in_dir(path_dirs[0] + 5, cmd_args[0]);
    while (cmd_args[0] && path_dirs[dir_idx] && cmd_path == NULL)
        cmd_path = find_cmd_in_dir(path_dirs[dir_idx++], cmd_args[0]);
    if (cmd_path != NULL)
        exit_code = execute_binary(cmd_path, cmd_args, env_list, shell);
    else
        exit_code = execute_binary(cmd_args[0], cmd_args, env_list, shell);
    free_tab(path_dirs);
    mem_free(cmd_path);
    return (exit_code);
}