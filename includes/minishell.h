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
extern t_sig	g_signal;

/* ========================= FUNCTION DECLARATIONS ========================= */

/* Built-in commands */
/* CD builtin */
int					ft_cd(char **args, t_mini *mini);
void				set_env_var(t_env *env, const char *var, const char *value);
char				*extract_path_value(t_env *env, const char *var,
						size_t len);
int					handle_oldpwd_update(t_env *env, char *cwd);
int					update_oldpwd(t_env *env);
int					handle_path_option(int option, t_env *env, char **env_path);
void				print_cd_error(char *arg, int error_type);
int					update_pwd(t_env *env, char *oldpwd);
int					go_to_home(t_env *env, char *oldpwd);
int					go_to_oldpwd(t_env *env, char *oldpwd);
int					go_to_path(char *path, t_env *env, char *oldpwd);
int					cd_get_oldpwd(char **oldpwd);
int					ft_echo(char **args);
int					ft_env(t_env *env);
int					ft_exit(char **args, t_mini *mini);
int					is_valid_number(char *str);
void				handle_numeric_arg(t_mini *mini, char *arg);
int					handle_invalid_arg(char **args, t_mini *mini);
int					check_too_many_args(char **args, t_mini *mini);
int					ft_export(char **args, t_mini *mini);
int					count_env_vars(t_env *env);
int					add_env_var(const char *var_value, t_env *env_list);
int					update_existing_var(t_env *env_list, char *new_var);
int					add_to_both_envs(char *cmd_arg, t_env *env_list,
						t_env *secret_env, int validation_result);
int					process_export_arg(char *cmd_arg, t_env *env_list,
						t_env *secret_env);
void				display_sorted_env_print(char **env_arr, int count);
void				display_sorted_env(t_env *env);
int					export_display_error(int error_type, const char *argument);
char				*export_extract_var_name(char *dest_buffer,
						const char *env_string);
int					export_is_valid_env(char *arg);
int					ft_pwd(void);
int					ft_unset(char **args, t_mini *mini);
int					is_valid_identifier_char(char c);
int					validate_identifier(const char *arg);
size_t				get_var_name_len(char *env_entry);
int					remove_first_node(t_mini *mini, t_env *current_env,
						char *cmd_arg);
int					remove_middle_node(t_mini *mini, t_env *current_env,
						char *cmd_arg);
void				remove_env_node(t_mini *mini, t_env *node_to_remove);
int					unset_variable(char *cmd_arg, t_mini *mini);

/* Environment functions */
char				*get_env_path(t_env *env, const char *var, size_t len);
int					setup_env_list(t_mini *shell, char **env_arr);
char				**env_list_to_array(t_env *env_list);
t_env				*env_dup(t_env *env);
char				*get_env(char *var, t_env *env);
void				shell_level(t_env *env);
t_env				*create_env_node(char *value);
void				set_env_var(t_env *env, const char *key, const char *value);
char				*create_new_env_var(const char *key, const char *value);
int					update_env_var(t_env *env, const char *key,
						char *new_env_var, int key_len);
int					setup_empty_env(t_mini *shell);
int					create_env_list(t_mini *shell, char **env_arr);
int					is_env_char(int character);
int					is_valid_env(const char *env_str);
char				*find_env_value(char *var_name, t_env *env_list);
void				increment_shell_level(t_env *env_list);

/* Input loop */
void				input_loop(struct s_mini *shell);
void				handle_input(t_mini *shell, char *input);
void				exec_pipeline(t_mini *shell);
void				free_args(char **args);
char				**token_to_args(t_token *token);
int					execute_builtin(t_mini *shell, char **args, t_token *token);
int					exec_builtin(char **cmd_args, t_mini *shell);
void				exec_bin(char **args, t_mini *mini);
void				handle_redirection(t_mini *shell, t_token *token, int type);
char				*handle_expansions(char *input, t_env *env, int ret);
void				reset_standard_fds(t_mini *shell);
void				setup_readline(void);
char				*read_input(void);
void				apply_redirections(t_mini *shell, t_token *token);
void				restore_stdout(t_mini *shell, int stdout_copy);
void				handle_redir_exec(t_mini *shell, t_token *token);
void				cleanup_tokens_env(t_mini *shell);
void				cleanup_shell(t_mini *shell);
void				setup_pipe_child(int prev_fd, int pipe_fd[2],
						t_token *current);
pid_t				process_pipe_segment(t_mini *shell, t_token **cmd_start,
						t_token *current, int *prev_fd);

/* Executor */
void				exec_cmd(t_mini *shell, t_token *token);
void				execute_command(t_mini *shell, char **cmd_args);
void				expand_command_args(char **cmd_args, t_mini *shell);
void				executor_utils(t_mini *mini, t_token *token);
char				**token_list_to_array(t_token *token);
int					is_redirection(int type);
void				redir(t_mini *mini, t_token *token, int type);
void				input_redirection(t_mini *mini, t_token *token);
int					minipipe(t_mini *mini);
void				reset_fds(t_mini *shell);
void				increment_shell_level(t_env *env);
int					setup_secret_env(t_mini *shell, char **env);

/* Executor functions */
void				exec_cmd(t_mini *shell, t_token *token);
int					minipipe(t_mini *mini);
int					display_error_msg(char *cmd_path);
char				*join_path(const char *dir, const char *filename);
char				*find_cmd_in_dir(char *dir_path, char *cmd_name);
int					handle_bin_child_process(char *bin_path, char **cmd_args,
						t_env *env_list);
int					execute_binary(char *bin_path, char **cmd_args,
						t_env *env_list);
int					handle_path_search(char **path_dirs, char **cmd_args,
						t_env *env_list);
char				*get_env_value(t_env *env_list, const char *key);
int					is_valid_fd(int fd);
void				setup_builtin_redirs(t_mini *shell);
void				restore_std_fds(int saved_stdout, int saved_stdin);
void				setup_child_signals_and_fds(t_mini *shell);
void				handle_parent_wait(pid_t pid, t_mini *shell);

/* Parser */
void				expand_utils(t_mini *mini, t_token *token);
void				parse_line(t_mini *mini, char *line);
t_token				*tokenizer(char *line);
void				parsing(t_mini *mini, t_token *token);
void				parsing_utils(t_mini *mini, t_token *token);
void				process_line_tokenize(t_mini *mini, char *line);
int					process_line_heredoc(t_mini *mini);
int					varlcpy(char *new_arg, const char *env_value, int pos);
void				handle_var_increment(char *arg, int *j);
void				insert_var(t_expansions *ex, char *arg, t_env *env,
						int ret);
void				process_expansion(t_expansions *ex, char *arg, t_env *env,
						int ret);
int					handle_signal_check(t_mini *mini, char *line);
int					handle_quote_check(t_mini *mini, char **line);
void				handle_heredoc_sep(char *line, char *new, int *i, int *j);
void				handle_other_sep(char *line, char *new, int *i, int *j);
void				handle_whitespace_seq(char *line, char *new, int *i,
						int *j);
void				handle_escaped_char(char *line, char *new, int *i, int *j);
void				handle_quoted_str(char *line, char *new, int *i, int *j);
void				setup_signals(void);
char				*read_input_line(t_mini *mini);
int					handle_empty_line(char *line);
void				add_line_to_history(t_mini *mini, char *line);
int					handle_exit_command(char *line, t_mini *mini);
int					handle_escaped_quote(char **line, int *i);
int					handle_quote_pair(char **line, int *i, t_mini *mini);
void				print_syntax_error_token(t_token *token);
int					validate_redir_token(t_mini *mini, t_token *token);
int					validate_pipe_end_token(t_mini *mini, t_token *token);
int					validate_token(t_mini *mini, t_token *token);
int					check_line(t_mini *mini, t_token *token);
int					add_tokens_from_args(t_token **tokens, char **args);

/* Heredoc */
int					contains_heredoc(t_token *tokens);
int					is_heredoc_delimiter(char *line, char *delimiter);
int					read_heredoc_content(const char *delimiter, int pipefd[2]);
char				*read_heredoc_input(const char *delimiter);
int					handle_heredoc(t_mini *mini, t_token *token);
int					handle_heredoc_input(t_mini *mini, t_token *tokens);
int					process_heredoc_content(t_mini *mini, char *delimiter,
						int pipefd[2]);
char				*process_heredoc(char *input);
void				display_eof_warning(const char *delimiter);
int					write_heredoc_line(int fd, char *line);
int					read_heredoc_lines(const char *delimiter, int pipefd[2]);
int					setup_heredoc_pipe(int pipefd[2]);
int					cleanup_heredoc_resources(int pipefd[2], int status);
int					update_mini_fdin(t_mini *mini, int pipe_read_end);
int					process_single_heredoc(t_mini *mini, t_token *token);

/* Signal handling */
void				sig_int(int code);
void				sig_quit(int code);
void				sig_init(void);

/* Utils */
void				fd_putstr(int fd, char *str);
char				**command_completion(const char *text, int start, int end);
void				free_token(t_token *token);
void				free_env(t_env *env);
void				free_mini(t_mini *mini);
void				free_history(t_history *history);
void				free_history_list(t_history *history);
void				free_all(t_mini *mini);
void				free_token_list(t_token *token);
void				free_env_list(t_env *env);
void				free_minishell(t_mini *mini);

/* Input */
char				*read_input(void);
void				process_input(t_mini *mini, char *input);
char				*read_line_with_history(t_mini *mini);
void				add_to_history(t_mini *mini, char *line);
int					process_line(t_mini *mini, char *buffer, int bytes_read);

/* Token */
t_token				*create_token(char *str, int type);
void				add_token(t_token **tokens, t_token *new);
t_token				*last_token(t_token *tokens);
void				free_tokens(t_token *tokens);
int					count_tokens(t_token *tokens);
void				type_arg(t_token *token, int separator);
void				move_token_to_prev(t_token *token, t_token *prev);
t_token				*next_sep(t_token *token, int skip);
t_token				*prev_sep(t_token *token, int skip);
t_token				*next_run(t_token *token, int skip);
int					count_token_args(t_token *token);
char				**token_to_args(t_token *token);
void				free_args(char **args);

/* Type */
int					is_builtin(char *cmd);
int					is_redirection(int type);
int					is_pipe(int type);
int					has_pipe(t_token *token);
int					is_quote(char c);
int					is_space(char c);
int					is_special(char c);

/* Expansions */
char				*expansions(char *arg, t_env *env, int ret);
char				*expand_vars(char *input, t_env *env, int ret);
char				*expand_tilde(char *input, t_env *env);
char				*expand_dollar(char *input, t_env *env, int ret);
int					ret_size(int ret);
int					get_var_len(char *arg, int pos, t_env *env, int ret);
char				*get_var_value(char *arg, int pos, t_env *env, int ret);
void				process_expansion_len(char *arg, int *i, int *size,
						t_env *env);
int					arg_alloc_len(char *arg, t_env *env, int ret);
int					is_env_char(int character);
int					arg_alloc_len(char *arg, t_env *env, int ret);
void				*mem_free(void *ptr);
char				*expand_arithmetic(char *arg, t_mini *mini);
char				*expand_command(char *arg, t_mini *mini);
char				*expand_redirections(char *arg, t_mini *mini);
char				*expand_heredoc(char *arg, t_mini *mini);
char				*expand_herestring(char *arg, t_mini *mini);
char				*expand_process(char *arg, t_mini *mini);

/* Terminal */
void				init_terminal(t_history_ctx *ctx);
void				reset_terminal(t_history_ctx *ctx);
t_history_ctx		*get_history_ctx(t_history_ctx *ctx);

/* Memory */
void				*ft_memalloc(size_t size);
void				ft_memdel(void **ap);
void				*ft_realloc(void *ptr, size_t size);

/* Error handling */
void				print_error(char *msg);
void				print_error_exit(char *msg, int exit_code);
void				print_syntax_error(char *token);
void				print_command_not_found(char *cmd);
void				print_permission_denied(char *cmd);
void				print_no_such_file(char *file);
void				print_is_a_directory(char *file);
void				print_not_a_directory(char *file);
void				print_too_many_arguments(char *cmd);
void				print_numeric_required(char *arg);
void				print_invalid_option(char *cmd, char *option);
char				*find_env_value(char *var_name, t_env *env_list);
void				print_tokens(t_token *tokens);
void				print_env(t_env *env);
void				print_mini(t_mini *mini);
char				*space_alloc(char *line);
void				handle_quotes_and_spaces(char *line, char *new, int *i,
						int *j);
char				*space_line(char *line);
int					quote_check(t_mini *mini, char **line);
char				*clean_input(char *buffer, int bytes_read);
void				process_tokens(t_mini *mini);
void				process_tokens_heredoc(t_mini *mini);
int					process_line(t_mini *mini, char *buffer, int bytes_read);
int					is_sep(char *line, int i);
int					is_whitespace(char c);
void				type_arg(t_token *token, int separator);
t_token				*get_tokens(char *line);
void				squish_args(t_mini *mini);
int					is_escaped_char(char c1, char c2);
int					process_escaped_sequence(char *line, int *i,
						int *count);
int					next_alloc(char *line, int *i);
int					contains_heredoc(t_token *tokens);
int					handle_heredoc(t_mini *mini, t_token *token);
int					read_heredoc_content(const char *delimiter,
						int pipefd[2]);
char				*read_heredoc_input(const char *delimiter);
t_token				*prev_sep(t_token *token, int skip);
int					is_type(t_token *token, int type);
int					is_types(t_token *token, char *types);
int					is_last_valid_arg(t_token *token);
void				move_token_to_prev(t_token *token, t_token *prev);
int					is_sep(char *line, int i);
int					quotes(char *line, int index);

#endif
