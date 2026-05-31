#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_sh			t_sh;
typedef struct s_command	t_command;
typedef struct s_redir		t_redir;
typedef struct s_ast_node	t_ast_node;

void	execute_ast(t_sh *sh, t_ast_node *node);
void	execute_cmd(t_sh *sh, t_command *cmd);
int		apply_redirections(t_sh *sh, t_redir *redirs);

// Utils
char	*resolve_path(t_sh *sh, char *cmd_name);
char	**env_to_envp(t_sh *sh);
void	close_fds(t_sh *sh, t_command *cmd);
void	setup_heredoc_fd(t_command *cmd);
void	handle_cmd_not_found(t_command *cmd);
void	wait_single_child(t_sh *sh, pid_t pid);

#endif
