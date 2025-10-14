/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 21:50:00 by aalegria          #+#    #+#             */
/*   Updated: 2025/10/13 21:50:00 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ============================== INCLUDES ================================= */

/* System headers */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Project headers */
# include "libft.h"

/* ============================== DEFINES ================================== */

/* Token types */
# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7
# define HEREDOC 8

/* File descriptors */
# define STDIN 0
# define STDOUT 1
# define STDERR 2

/* Control flags */
# define SKIP 1
# define NOSKIP 0

/* Buffer and status codes */
# define BUFF_SIZE 4096
# define EXPANSION -36
# define ERROR 1
# define SUCCESS 0
# define IS_DIRECTORY 126
# define UNKNOWN_COMMAND 127

/* ============================== STRUCTS ================================== */

typedef struct s_history
{
	char				*command;
	struct s_history	*next;
	struct s_history	*prev;
}	t_history;

typedef struct s_history_ctx
{
	struct termios	original_termios;
	t_history		*history;
	t_history		*current_hist;
}	t_history_ctx;

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
	int		saved_stdout;
	int		nb_pipe;
	int		should_execute;
	int		exit_code;
	int		nb_commands;
	int		should_expand;
	int		should_expand_args;
	int		should_expand_redir;
	int		should_expand_files;
	int		should_expand_heredoc;
	int		should_expand_quotes;
	int		should_expand_env;
	int		should_expand_tilde;
	int		should_expand_parameters;
	int		should_expand_arithmetic;
	int		should_expand_command;
	int		should_expand_redirections;
	int		should_expand_here_doc;
	int		should_expand_here_string;
	int		should_expand_process;
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

/* ========================== GLOBAL VARIABLE ============================= */

#ifdef GLOBAL_VARIABLE_DEFINITION
# define EXTERN
#else
# define EXTERN extern
#endif

EXTERN t_sig	g_sig;

/* ========================= FUNCTION DECLARATIONS ========================= */

/* Built-in commands */
/* CD builtin */
int				ft_cd(char **args, t_mini *mini);
void			set_env_var(t_env *env, const char *var, const char *value);
char			*extract_path_value(t_env *env, const char *var, size_t len);
int				handle_oldpwd_update(t_env *env, char *cwd);
int				update_oldpwd(t_env *env);
int				handle_path_option(int option, t_env *env, char **env_path);
void			print_cd_error(char *arg, int error_type);
int				update_pwd(t_env *env, char *oldpwd);
int				go_to_home(t_env *env, char *oldpwd);
int				go_to_oldpwd(t_env *env, char *oldpwd);
int				go_to_path(char *path, t_env *env, char *oldpwd);
int				ft_echo(char **args);
int				ft_env(t_env *env);
int				ft_exit(char **args, t_mini *mini);
int				ft_export(char **args, t_mini *mini);
int				ft_pwd(void);
int				ft_unset(char **args, t_mini *mini);

/* Environment functions */
char			*get_env_path(t_env *env, const char *var, size_t len);
int				setup_env_list(t_mini *shell, char **env_arr);
char			**env_list_to_array(t_env *env_list);
t_env			*env_dup(t_env *env);
char			*get_env(char *var, t_env *env);
void			shell_level(t_env *env);

/* Input loop */
void			input_loop(struct s_mini *shell);
void			handle_input(t_mini *shell, char *input);
void			free_args(char **args);
char			**token_to_args(t_token *token);
int				execute_builtin(t_mini *shell, char **args, t_token *token);
int				exec_builtin(char **cmd_args, t_mini *shell);
void			exec_bin(char **args, t_mini *mini);
void			handle_redirection(t_mini *shell, t_token *token, int type);
char			*handle_expansions(char *input, t_env *env, int ret);
void			reset_standard_fds(t_mini *shell);

/* Executor */
void			exec_cmd(t_mini *shell, t_token *token);
void			executor_utils(t_mini *mini, t_token *token);
char			**token_list_to_array(t_token *token);
int				is_redirection(int type);
void			redir(t_mini *mini, t_token *token, int type);
void			input_redirection(t_mini *mini, t_token *token);
int				minipipe(t_mini *mini);
void			reset_fds(t_mini *shell);
void			increment_shell_level(t_env *env);
int				setup_secret_env(t_mini *shell, char **env);

/* Parser */
void			expand_utils(t_mini *mini, t_token *token);
void			parse_line(t_mini *mini, char *line);
t_token			*tokenizer(char *line);
void			parsing(t_mini *mini, t_token *token);
void			parsing_utils(t_mini *mini, t_token *token);

/* Heredoc */
int				contains_heredoc(t_token *tokens);
int				is_heredoc_delimiter(char *line, char *delimiter);
int				read_heredoc_content(const char *delimiter, int pipefd[2]);
char			*read_heredoc_input(const char *delimiter);
int				handle_heredoc(t_mini *mini, t_token *token);
int				handle_heredoc_input(t_mini *mini, t_token *tokens);
int				process_heredoc_content(t_mini *mini, char *delimiter,
					int pipefd[2]);
char			*process_heredoc(char *input);

/* Signal handling */
void			sig_int(int code);
void			sig_quit(int code);
void			sig_init(void);

/* Utils */
void			fd_putstr(int fd, char *str);
char			**command_completion(const char *text, int start, int end);
static inline void	free_array(char **array) { ft_free_array(array); }
void			free_token(t_token *token);
void			free_env(t_env *env);
void			free_mini(t_mini *mini);
void			free_history(t_history *history);
void			free_all(t_mini *mini);
void			free_token_list(t_token *token);
void			free_env_list(t_env *env);
void			free_minishell(t_mini *mini);

/* Input */
char			*read_input(void);
void			process_input(t_mini *mini, char *input);
char			*read_line_with_history(t_mini *mini);
void			add_to_history(t_mini *mini, char *line);
int				process_line(t_mini *mini, char *buffer, int bytes_read);

/* Token */
t_token			*create_token(char *str, int type);
void			add_token(t_token **tokens, t_token *new);
t_token			*last_token(t_token *tokens);
void			free_tokens(t_token *tokens);
int				count_tokens(t_token *tokens);

/* Type */
int				is_builtin(char *cmd);
int				is_redirection(int type);
int				is_pipe(int type);
int				is_quote(char c);
int				is_space(char c);
int				is_special(char c);

/* Expansions */
char			*expand_vars(char *input, t_env *env, int ret);
char			*expand_tilde(char *input, t_env *env);
char			*expand_dollar(char *input, t_env *env, int ret);
int				ret_size(int ret);
int				get_var_len(char *arg, int pos, t_env *env, int ret);
char			*get_var_value(char *arg, int pos, t_env *env, int ret);
int				is_env_char(int character);
int				arg_alloc_len(char *arg, t_env *env, int ret);
void			*mem_free(void *ptr);
char			*expand_arithmetic(char *arg, t_mini *mini);
char			*expand_command(char *arg, t_mini *mini);
char			*expand_redirections(char *arg, t_mini *mini);
char			*expand_heredoc(char *arg, t_mini *mini);
char			*expand_herestring(char *arg, t_mini *mini);
char			*expand_process(char *arg, t_mini *mini);

/* Terminal */
void			init_terminal(t_history_ctx *ctx);
void			reset_terminal(t_history_ctx *ctx);

/* Memory */
void			*ft_memalloc(size_t size);
void			ft_memdel(void **ap);
void			*ft_realloc(void *ptr, size_t size);

/* Error handling */
void			print_error(char *msg);
void			print_error_exit(char *msg, int exit_code);
void			print_syntax_error(char *token);
void			print_command_not_found(char *cmd);
void			print_permission_denied(char *cmd);
void			print_no_such_file(char *file);
void			print_is_a_directory(char *file);
void			print_not_a_directory(char *file);
void			print_too_many_arguments(char *cmd);
void			print_numeric_required(char *arg);
void			print_invalid_option(char *cmd, char *option);
char			*find_env_value(char *var_name, t_env *env_list);
void			print_tokens(t_token *tokens);
void			print_env(t_env *env);
void			print_mini(t_mini *mini);
char			*space_alloc(char *line);
void			handle_quotes_and_spaces(char *line, char *new, int *i, int *j);
char			*space_line(char *line);
int				quote_check(t_mini *mini, char **line);
char			*clean_input(char *buffer, int bytes_read);
void			process_tokens(t_mini *mini);
void			process_tokens_heredoc(t_mini *mini);
int				process_line(t_mini *mini, char *buffer, int bytes_read);
int				is_sep(char *line, int i);
int				is_whitespace(char c);
void			type_arg(t_token *token, int separator);
t_token			*get_tokens(char *line);
void			squish_args(t_mini *mini);
int				is_escaped_char(char c1, char c2);
int				process_escaped_sequence(char *line, int *i, int *count);
int				next_alloc(char *line, int *i);
int				contains_heredoc(t_token *tokens);
int				handle_heredoc(t_mini *mini, t_token *token);
int				read_heredoc_content(const char *delimiter, int pipefd[2]);
char			*read_heredoc_input(const char *delimiter);
t_token			*prev_sep(t_token *token, int skip);
int				is_type(t_token *token, int type);
int				is_types(t_token *token, char *types);
int				is_last_valid_arg(t_token *token);
void			move_token_to_prev(t_token *token, t_token *prev);
int				is_sep(char *line, int i);
int				quotes(char *line, int index);

#endif
