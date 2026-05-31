#ifndef BUILTIN_H
# define BUILTIN_H

# define MAX_PATH 4048
# define LLONG_MAX		9223372036854775807LL
# define LLONG_MIN		-9223372036854775807LL

typedef struct s_sh	t_sh;//circular include problemi böyle çözülürmüş

typedef struct s_pwd
{
	char			*value;
	struct s_pwd	*next;
}	t_pwd;

typedef struct s_cd
{
	t_env	*oldpwd_adr;
	t_env	*pwd_adr;
	char	tmp_pwd[MAX_PATH];
	int		tmp_pwd_f;
}	t_cd;

void	builtin_env(t_sh *sh, char **param);

void	builtin_exit(t_sh *sh, char **param);
int		validate_parameter(t_sh *sh, char *param);
long	ft_atol(t_sh *sh, char *param, int *flag);

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
t_env	*find_env(t_sh *sh, char *name);
int		count_max_lvl(char **base, char **path);
void	update_env_value(t_sh *sh, char *name, char *new_value, int has_value);
void	create_oldpwd(t_sh *sh, t_env *new_oldpwd);
int		cd_home(t_sh *sh, char **param);
void	absolute_pwd_update(t_sh *sh, t_env **curr_pwd, char *param);
char	*build_path(t_sh *sh, char **base, int b_lvl);
void	calculate_new_pwd(t_sh *sh, char ***result, char **base, char **path);
void	create_pwd(t_sh *sh, char *param, t_env *new_oldpwd);
void	logical_pwd_update(t_sh *sh, char *param, t_env *new_oldpwd);
void	update_pwds(t_sh *sh, char *param);

void	builtin_pwd(t_sh *sh, char **param);

int		identify_builtin(char *cmd_name);
int		exec_builtin(t_sh *sh, t_command *cmd);

// Variable Assignment
int		is_assignment(char *str);
int		exec_assignment(t_sh *sh, char *str);
int		handle_assignments(t_sh *sh, t_command *cmd);
char	**var_delimeter(t_sh *sh, char *envp, int i);
int		permanent_update(t_sh *sh, char *str);

#endif