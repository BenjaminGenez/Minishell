/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:50 by user              #+#    #+#             */
/*   Updated: 2025/09/12 10:39:50 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_token *handle_operator(char **input, int *i)
{
    t_token *token;
    char    op[3];
    int     j;

    j = 0;
    op[0] = (*input)[*i];
    op[1] = '\0';
    
    // Manejar operadores de dos caracteres (>>, <<)
    if (((*input)[*i] == '>' && (*input)[*i + 1] == '>') ||
        ((*input)[*i] == '<' && (*input)[*i + 1] == '<'))
    {
        op[1] = (*input)[*i + 1];
        op[2] = '\0';
        (*i)++;
    }
    
    token = create_token(ft_strdup(op), get_token_type(op));
    (*i)++;
    return (token);
}

static t_token *handle_word(char **input, int *i)
{
    int     start;
    char    *word;
    t_token *token;
    char    quote;
    int     in_quotes;

    start = *i;
    in_quotes = 0;
    quote = '\0';
    
    while ((*input)[*i])
    {
        if (((*input)[*i] == '\'' || (*input)[*i] == '"') && !in_quotes)
        {
            quote = (*input)[*i];
            in_quotes = 1;
            (*i)++;
            continue;
        }
        else if ((*input)[*i] == quote && in_quotes)
        {
            in_quotes = 0;
            quote = '\0';
            (*i)++;
            continue;
        }
        
        if (!in_quotes && (ft_isspace((*input)[*i]) || is_metacharacter((*input)[*i])))
            break;
            
        (*i)++;
    }
    
    word = ft_substr(*input, start, *i - start);
    token = create_token(word, TOKEN_WORD);
    return (token);
}

t_token *tokenize(char *input)
{
    t_token *tokens;
    t_token *new_token;
    int     i;

    if (!input || !*input)
        return (NULL);
    
    tokens = NULL;
    i = 0;
    
    while (input[i])
    {
        if (ft_isspace(input[i]))
        {
            i++;
            continue;
        }
        
        if (is_metacharacter(input[i]))
            new_token = handle_operator(&input, &i);
        else
            new_token = handle_word(&input, &i);
            
        if (!new_token)
        {
            free_tokens(&tokens);
            return (NULL);
        }
        
        add_token(&tokens, new_token);
    }
    
    // Añadir token de fin de archivo
    add_token(&tokens, create_token(NULL, TOKEN_EOF));
    
    return (tokens);
}

void free_tokens(t_token **tokens)
{
    t_token *current;
    t_token *next;

    if (!tokens || !*tokens)
        return;
        
    current = *tokens;
    while (current)
    {
        next = current->next;
        if (current->value)
            free(current->value);
        free(current);
        current = next;
    }
    *tokens = NULL;
}

void print_tokens(t_token *tokens)
{
    const char *token_types[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
        "APPEND", "HEREDOC", "QUOTE", "DQUOTE",
        "WHITESPACE", "EOF"
    };
    
    printf("=== TOKENS ===\n");
    while (tokens)
    {
        printf("Type: %-10s | Value: %s\n", 
               token_types[tokens->type],
               tokens->value ? tokens->value : "(null)");
        tokens = tokens->next;
    }
    printf("==============\n");
}

t_token *create_token(char *value, t_token_type type)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
        
    token->value = value;
    token->type = type;
    token->next = NULL;
    
    return (token);
}

void add_token(t_token **tokens, t_token *new_token)
{
    t_token *current;

    if (!new_token)
        return;
        
    if (!*tokens)
    {
        *tokens = new_token;
        return;
    }
    
    current = *tokens;
    while (current->next)
        current = current->next;
        
    current->next = new_token;
}

int is_metacharacter(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t' || 
            c == '\n' || c == '\'' || c == '"');
}

t_token_type get_token_type(char *value)
{
    if (!value)
        return (TOKEN_EOF);
        
    if (ft_strcmp(value, "|") == 0)
        return (TOKEN_PIPE);
    else if (ft_strcmp(value, "<") == 0)
        return (TOKEN_REDIR_IN);
    else if (ft_strcmp(value, ">") == 0)
        return (TOKEN_REDIR_OUT);
    else if (ft_strcmp(value, ">>") == 0)
        return (TOKEN_APPEND);
    else if (ft_strcmp(value, "<<") == 0)
        return (TOKEN_HEREDOC);
    else if (ft_strcmp(value, "\'") == 0)
        return (TOKEN_QUOTE);
    else if (ft_strcmp(value, "\"") == 0)
        return (TOKEN_DQUOTE);
    else if (ft_isspace(*value))
        return (TOKEN_WHITESPACE);
    
    return (TOKEN_WORD);
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}
