/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:50 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:39:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// Tipos de tokens
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_QUOTE,
    TOKEN_DQUOTE,
    TOKEN_WHITESPACE,
    TOKEN_EOF
}   t_token_type;

// Estructura de un token
typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
}   t_token;

// Prototipos de funciones del parser
// Tokenizador
t_token     *tokenize(char *input);
void        free_tokens(t_token **tokens);
void        print_tokens(t_token *tokens);

// Parser
int         parse_input(t_shell *shell, char *input);
t_cmd       *parse_pipeline(t_token **tokens);
t_cmd       *parse_command(t_token **tokens);
int         parse_redirections(t_cmd *cmd, t_token **tokens);

// Utils
t_token     *create_token(char *value, t_token_type type);
void        add_token(t_token **tokens, t_token *new_token);
int         is_metacharacter(char c);
t_token_type get_token_type(char *value);
int         is_redirection(t_token_type type);

// Expansión de variables
int         expand_variables(t_shell *shell, t_cmd *cmd);
char        *expand_env_vars(t_shell *shell, char *str);
char        *handle_quotes(t_shell *shell, char *str, int *i, char quote);
char        *handle_dollar(t_shell *shell, char *str, int *i);

#endif
