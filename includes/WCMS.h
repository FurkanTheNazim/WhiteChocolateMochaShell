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

//çevre değişkenlerinin saklanacağı ve güncelleneceği liste
typedef struct s_env
{
	char			*env_name;
	char			*env_value;
	struct s_env	*next;
} t_env;

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
} t_sh;

#endif