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

typedef enum e_builtin_type
{
	NOT_BUILTIN,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}	t_builtin_type;

// t_token: Lexer'ın ürettiği her bir token'ı temsil eder
// raw: Orijinal string (örn: "hello")
// value: Quote'suz işlenmiş değer (örn: hello)  
// quote_type: 0=yok, 1=single, 2=double, 3=mixed
// type: TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_REDIR_APPEND, TOKEN_HEREDOC
// next: Sonraki token'ı gösterir

typedef struct s_token
{
	char			*raw;
	char			*value;
	int				type;
	int				quote_type;
	int				is_heredoc;
	char			*heredoc_file;
    struct s_token	*next;
} t_token;

// t_lexer_state: Lexer'ın durumunu tutacak
// in_double_quote ve in_single_quote bool değerleri olacak
// i: input string'indeki mevcut pozisyon (index)
// input string'ini tutacak
typedef struct s_lexer
{
    int		in_double_quote;
    int		in_single_quote;
    int		i;
    char	*input;
}   t_lexer;

typedef struct s_redir
{
	int				type;
	char			*file;
	int				heredoc_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
    char    **args;
    t_redir *redirs;
    int     builtin;
    int     is_heredoc;
    char	*heredoc_file;
    struct s_command *next;
}   t_command;

typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE
}	t_ast_type;

typedef struct s_ast_node
{
	t_ast_type			type;
	t_command			*cmd;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

// Lexer Utils
int				is_operator(char c);
int				ft_isspace(char c);
void			init_lexer_stat(t_lexer *state);

// Lexer Token
t_token			*newnode(t_sh *sh, char *raw, char *token);
int             addback(t_sh *sh, t_token *newnode);
void			free_node(t_sh *sh, t_token **list);

// Lexer Operators
int				append_operator(char *ptr, t_sh *sh);

// Lexer Main
t_token			*lexer(t_sh *sh);

// Init
void			init_minishell(t_sh *sh);

// Parser Utils
t_command *create_command(t_sh *sh);
void add_arg(t_sh *sh, t_command *cmd, t_token *token);
void add_command(t_sh *sh, t_command **list, t_command *new);
void free_command_list(t_sh *sh, t_command *list);

t_redir *create_redir(t_sh *sh, int type, char *file);
void add_redir(t_sh *sh, t_command *cmd, t_redir *new);
void free_redir_list(t_sh *sh, t_redir *list);

// Expander
// int expander(t_sh *sh);


// Parser
t_command	*parser(t_sh *sh);
int			validate_tokens(t_sh *sh);

// AST
t_ast_node	*build_ast(t_sh *sh);
t_ast_node	*create_ast_cmd(t_sh *sh, t_command *cmd);
t_ast_node	*create_ast_pipe(t_sh *sh, t_ast_node *left, t_ast_node *right);
void		print_ast(t_ast_node *node, int depth);


#endif  
