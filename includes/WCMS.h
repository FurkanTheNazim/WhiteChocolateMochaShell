#ifndef WCMS_H
# define WCMS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include "libft/libft.h"
# include "lexer_parser.h"
# include "gc.h"
# include "env.h"
# include "builtin.h"
# include "expander.h"

typedef struct s_sh
{
    t_token	*token_list;
    t_gc	*gc;
	t_env	*env;
    t_command *command_list;
    char	**envp;
    char	*input;
	int		exit_status;
	int		syntax_error;
    int     default_path;
    int     in_pipe;
} t_sh;

#endif