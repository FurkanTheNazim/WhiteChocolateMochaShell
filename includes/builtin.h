#ifndef BUILTIN_H
# define BUILTIN_H

# define MAX_PATH 4048
# define LLONG_MAX  9223372036854775807LL
# define LLONG_MIN  (-LLONG_MAX - 1LL)

typedef struct s_sh t_sh;//circular include problemi böyle çözülürmüş

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

void	builtin_env(t_sh *sh, char **param);

void	builtin_exit(t_sh *sh, char **param);
int		validate_parameter(t_sh *sh, char *param);
long	ft_atol(char *param, int *flag);
void	exit_err(char *err, char *param);

void	builtin_unset(t_sh *sh, char **param);
void	env_del_node(t_sh *sh, t_env *list);

void	builtin_export(t_sh *sh, char **param);
int		check_list(t_sh *sh, char **param);
char	**get_name_value(t_sh *sh, char *str);
void	declare(t_sh *sh);
int		param_count(char **param);
int		validate_param(t_sh *sh, char *str);
void	export_err(t_sh *sh, char	*param);

void	builtin_echo(t_sh *sh, char **param);
int		validate_flag(char *flag);

void	builtin_cd(t_sh *sh, char **param);
int		cd_home(t_sh *sh, char **param);
void	update_pwds(t_sh *sh, char *param);
t_env	*find_env(t_sh	*sh, char *name);
void	logical_pwd_update(t_sh *sh, char *param, t_env *new_oldpwd);
void	absolute_pwd_update(t_sh *sh, t_env **curr_pwd, char *param);
void	create_pwd(t_sh *sh, char *param, t_env *new_oldpwd);
int		count_matrix(char **martix);
void	update_env_value(t_sh *sh, char *name, char *new_value, int has_value);
char	*build_path(t_sh *sh, char **base, int b_lvl);

void    builtin_pwd(t_sh *sh, char **param);

int		identify_builtin(char *cmd_name);
int		exec_builtin(t_sh *sh, t_command *cmd);

// Variable Assignment
int		is_assignment(char *str);
void	exec_assignment(t_sh *sh, char *str);
int		handle_assignments(t_sh *sh, t_command *cmd);

#endif