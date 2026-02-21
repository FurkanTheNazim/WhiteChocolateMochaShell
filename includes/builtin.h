#ifndef BUILTIN_H
# define BUILTIN_H

# define MAX_PATH 4048

typedef struct s_pwd
{
	char	*value;
	struct s_pwd *next;
}t_pwd;

typedef	struct	s_cd
{
	t_env	*oldpwd_adr;
	t_env	*pwd_adr;
	char	tmp_pwd[MAX_PATH];
	int		tmp_pwd_f;
}t_cd;

void	builtin_env(t_sh *sh, char *param);


#endif