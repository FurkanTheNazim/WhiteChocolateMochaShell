#ifndef LEXER_PARSER_H
# define LEXER_PARSER_H

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_OR
}	t_token_type;

typedef enum e_quote_type
{
    N_QUOTE,
    S_QUOTE,
    D_QUOTE
}	t_quote_type;


// t_token: Lexer'ın ürettiği her bir token'ı temsil eder
// raw: Orijinal string (örn: "hello")
// value: Quote'suz işlenmiş değer (örn: hello)  
// quote_type: 0=yok, 1=single, 2=double, 3=mixed
// type: TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_REDIR_APPEND, TOKEN_HEREDOC
// next: Sonraki token'ı gösterir

typedef struct s_token
{
    char *raw;
    char *value;
    int type;
    int quote_type;
    struct s_token *next;
} t_token;

// t_lexer_state: Lexer'ın durumunu tutacak
// in_double_quote ve in_single_quote bool değerleri olacak
// i: input string'indeki mevcut pozisyon (index)
// input string'ini tutacak
typedef struct s_lexer_state
{
    int in_double_quote;
    int in_single_quote;
    int i;
    char *input;
}   t_lexer_state;

#endif  