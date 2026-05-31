#ifndef HEREDOC_H
# define HEREDOC_H

typedef struct s_sh		t_sh;
typedef struct s_token	t_token;

typedef struct s_heredoc
{
	int		flag;
	int		expand;
	char	*delimeter;
	t_token	*start;
	t_token	*end;

}	t_heredoc;

int		heredoc(t_sh *sh);
int		heredoc_child(t_sh *sh, t_heredoc *data, int fd);
void	read_heredoc_input(t_sh *sh, t_heredoc *data, int fd);
char	*expand_heredoc_input(t_sh *sh, int q_type, char *input);
int		handle_heredoc_eof(t_sh *sh, char *delimeter);
int		init_data(t_sh *sh, t_token *list, t_heredoc *data);
int		cr_heredoc_file(t_sh *sh, t_token *cmd);
char	*cr_file_name(t_sh *sh, int cmd_index);
void	find_cmd(t_token *list);
int		process_heredoc(t_sh *sh, t_token *list, t_heredoc *data, int fd);

#endif