#ifndef WCMS_H
#define WCMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include "libft/libft.h"
#include "lexer_parser.h" 

typedef struct s_minishell
{
    t_token *token_list;
    // t_command *command_list;
    char **envp;
    char *input;
    int exit_status;
    
} t_minishell;

#endif