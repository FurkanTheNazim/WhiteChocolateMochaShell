#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
    char *content;
    int type;
    struct s_token *next;
} t_token;

#endif  