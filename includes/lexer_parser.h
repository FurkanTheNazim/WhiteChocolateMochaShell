#ifndef LEXER_PARSER_H
# define LEXER_PARSER_H

typedef struct s_sh t_sh;

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
typedef struct s_lexer
{
    int in_double_quote;
    int in_single_quote;
    int i;
    char *input;
}   t_lexer;

typedef struct s_redir
{
    int type;
    char *file;
    struct s_redir *next;
}   t_redir;

typedef struct s_command
{
    char    **args;
    t_redir *redirs;
    struct s_command *next;
}   t_command;

// Lexer Utils
int				is_operator(char c);
int				ft_isspace(char c);
void			init_lexer_stat(t_lexer *state);

// Lexer Token
t_token			*create_newnode(t_sh *shell, char *raw, char *token);
void			addback(t_sh *shell, t_token *newnode);
void			free_node(t_token **list);

// Lexer Operators
int				append_operator(char *ptr, t_sh *shell);

// Lexer Main
t_token			*lexer(t_sh *shell);

// Init
void			init_minishell(t_sh *shell);

// Parser Utils
t_command *create_command(t_sh *shell);
void add_arg(t_sh *shell, t_command *cmd, char *arg);
void add_command(t_sh *shell, t_command **list, t_command *new);
void free_command_list(t_sh *shell, t_command *list);

t_redir *create_redir(t_sh *shell, int type, char *file);
void add_redir(t_sh *shell, t_command *cmd, t_redir *new);
void free_redir_list(t_sh *shell, t_redir *list);

// Expander
// int expander(t_sh *shell);


// Parser
t_command	*parser(t_sh *shell);
int			validate_tokens(t_sh *shell);

#endif  