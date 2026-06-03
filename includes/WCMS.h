#ifndef WCMS_H
# define WCMS_H

# define _XOPEN_SOURCE 700
# define _DEFAULT_SOURCE

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include "libft/libft.h"
# include "lexer_parser.h"
# include "gc.h"
# include "env.h"
# include "builtin.h"
# include "expander.h"
# include "errors.h"
# include "executor.h"
# include "sh_signals.h"
# include "heredoc.h"

extern volatile sig_atomic_t	g_sig;

typedef struct s_sh
{
	t_token		*token_list;
	t_gc		*gc;
	t_env		*env;
	t_command	*command_list;
	t_ast_node	*ast;
	t_heredoc	*heredoc;
	char		**envp;
	char		*input;
	int			exit_status;
	int			syntax_error;
	int			default_path;
	int			in_pipe;
	int			fds[2];
	int			newline;
	int			cmd_cnt;
}	t_sh;

void	get_input(t_sh *sh);
void	read_input(t_sh *sh);
int		newline_handler(t_sh *sh);
int		handle_eof(t_sh *sh);
int		check_ischar(char	*input);

#endif