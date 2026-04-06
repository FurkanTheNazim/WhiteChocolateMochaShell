#ifndef ENV_H
# define ENV_H

typedef struct s_sh t_sh;//circular include problemi böyle çözülürmüş

# define DEFAULT_PATH "/usr/local/bin:/usr/bin:/bin"

//çevre değişkenlerinin saklanacağı ve güncelleneceği liste
typedef struct s_env
{
	char			*env_name;
	char			*env_value;//temp
	int				exported;
	int				has_value;
	int				env_printable;
	int				temp_flag;
	char			old_value;//kalıcı
	struct s_env	*next;
} t_env;



t_env	*env_newnode(t_sh *sh, char *name, char *value);
void	init_env(t_sh *sh, char *av ,char **envp);
void	add_to_list(t_sh *sh, char *envp);
void	env_error(t_sh *sh, char *err);
void	manuel_init(t_sh *sh, char *av);
char	**env_delimeter(t_sh *sh, char *envp, int i);
int		validate_env(t_sh *sh);
int		env_addback(t_sh *sh, t_env *newnode);
void	normalize_env(t_sh *sh);

#endif
