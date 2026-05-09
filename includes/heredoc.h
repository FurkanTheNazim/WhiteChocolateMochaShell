#ifndef	HEREDOC_H
# define HEREDOC_H

typedef struct s_sh		t_sh;
typedef struct s_token	t_token;

typedef struct s_heredoc
{
	int		flag;
	int		expand;
	t_token	*start;
	t_token	*end;

}t_heredoc;


int isheredoc(t_sh *sh);

#endif