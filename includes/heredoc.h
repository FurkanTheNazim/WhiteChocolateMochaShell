#ifndef	HEREDOC_H
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

}t_heredoc;


int is_heredoc(t_sh *sh);
int	check_expand(t_sh *sh, t_heredoc *data);
int	cr_tmp_file(t_sh *sh, t_token *tmp, t_heredoc *data);
char	*expand_heredoc_input(t_sh *sh, int q_type, char *input);
char	*cr_file_name(t_sh *sh, int cmd_index);

#endif