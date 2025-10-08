/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:36:46 by user              #+#    #+#             */
/*   Updated: 2025/10/03 22:36:49 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define SKIP 1
# define NOSKIP 0

# define BUFF_SIZE 4096
# define EXPANSION -36
# define ERROR 1
# define SUCCESS 0
# define IS_DIRECTORY 126
# define UNKNOWN_COMMAND 127

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_mini
{
	t_token	*start;
	t_env	*env;
	t_env	*secret_env;
	int		in;
	int		out;
	int		fdin;
	int		fdout;
	int		pipin;
	int		pipout;
	int		pid;
	int		charge;
	int		parent;
	int		last;
	int		ret;
	int		exit;
	int		no_exec;
}	t_mini;

typedef struct s_sig
{
	int		sigint;
	int		sigquit;
	int		exit_status;
	pid_t	pid;
}	t_sig;

typedef struct s_expansions
{
	char	*new_arg;
	int		i;
	int		j;
}	t_expansions;

/*
** minishell
*/
void	redir(t_mini *mini, t_token *token, int type);
void	input(t_mini *mini, t_token *token);
int		minipipe(t_mini *mini);
char	*expansions(char *arg, t_env *env, int ret);

/*
** executor
*/
void	exec_cmd(t_mini *shell, t_token *token);
int		exec_bin(char **cmd_args, t_env *env_list, t_mini *shell);
char	**build_cmd_array(t_token *start_token);
void	cleanup_shell_fds(t_mini *shell);
int		exec_builtin(char **cmd_args, t_mini *shell);
int		is_builtin(char *cmd_name);
int		execute_binary(char *bin_path, char **cmd_args,
			t_env *env_list, t_mini *shell);
int		display_error_msg(char *cmd_path);
char	*find_cmd_in_dir(char *dir_path, char *cmd_name);

/*
** main
*/
int		main(int argc, char **argv, char **envp);
void	exec_pipeline(t_mini *shell);
void	cleanup_shell(t_mini *shell);

/*
** builtins
*/
int		ft_echo(char **args);
int		ft_cd(char **args, t_env *env);
int		ft_pwd(void);
int		ft_export(char **args, t_env *env, t_env *secret);
int		process_export_arg(char *cmd_arg, t_env *env_list, t_env *secret_env);
int		ft_env(t_env *env_list);
int		add_env_var(const char *var_value, t_env *env_list);
char	*extract_var_name(char *dest_buffer, const char *env_string);
int		update_existing_var(t_env *env_list, char *new_var);
int		ft_unset(char **args, t_mini *mini);
int		unset_variable(char *cmd_arg, t_mini *shell);
void	remove_env_node(t_mini *shell, t_env *node_to_remove);
void	mini_exit(t_mini *shell, char **cmd_args);

/*
** parser
*/
int		parse(t_mini *mini);
int		quote_check(t_mini *mini, char **line);
char	*space_line(char *line);
t_token	*get_tokens(char *line);
void	squish_args(t_mini *mini);
int		is_last_valid_arg(t_token *token);
int		quotes(char *line, int index);
void	type_arg(t_token *token, int separator);
int		is_sep(char *line, int i);
int		ignore_sep(char *line, int i);
char	*clean_input(char *buffer, int bytes_read);
void	process_tokens(t_mini *mini);
int		process_line(t_mini *mini, char *buffer, int bytes_read);

/*
** environment
*/
int		check_line(t_mini *mini, t_token *token);
char	*build_env_str(t_env *node);
int		setup_env_list(t_mini *shell, char **env_arr);
int		setup_secret_env(t_mini *shell, char **env_arr);
char	**env_list_to_array(t_env *env_list);
char	*find_env_value(char *var_name, t_env *env_list);
char	*extract_env_value(char *env_str);
int		calc_value_len(const char *env_str);
int		is_env_char(int character);
int		is_valid_env(const char *env_str);
void	print_sorted_env(t_env *env);
void	increment_shell_level(t_env *env);
size_t	calc_env_len(t_env *node);

/*
** cd builtin
*/
int		update_oldpwd(t_env *env);
int		handle_path_option(int option, t_env *env, char **env_path);
int		go_to_path(int option, t_env *env);

/*
** fd
*/
void	reset_std(t_mini *mini);
void	close_fds(t_mini *mini);
void	ft_close(int fd);
void	reset_fds(t_mini *mini);

/*
** free utils
*/
void	free_token(t_token *start);
void	free_env(t_env *env);
void	free_tab(char **tab);
void	*mem_free(void *ptr);

/*
** token
*/
/* Parsing functions */
int		parse(t_mini *mini);
int		check_line(t_mini *mini, t_token *token);
int		validate_token(t_mini *mini, t_token *token);

/* Token functions */
t_token	*create_token(char *str, int type);
void	free_tokens(t_token *tokens);
void	move_token_to_prev(t_token *token, t_token *prev);
t_token	*next_sep(t_token *token, int skip);
t_token	*prev_sep(t_token *token, int skip);
t_token	*next_run(t_token *token, int skip);

/*
** type utils
*/
int		is_type(t_token *token, int type);
int		is_types(t_token *token, char *types);
int		has_type(t_token *token, int type);
int		has_pipe(t_token *token);
t_token	*next_type(t_token *token, int type, int skip);

/*
** expansion
*/
int		ret_size(int ret);
int		get_var_len(const char *arg, int pos, t_env *env, int ret);
int		arg_alloc_len(const char *arg, t_env *env, int ret);
char	*get_var_value(const char *arg, int pos, t_env *env, int ret);

/*
** signal
*/
void	sig_int(int code);
void	sig_quit(int code);
void	sig_init(void);

extern t_sig	g_sig;
#endif