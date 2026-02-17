#include "WCMS.h"
#include "builtin.h"

void	builtin_pwd(t_sh *sh)
{
	char	*dir;
	t_env	*tmp;

	tmp = sh->env;
	dir = malloc(sizeof(char) * PATH_MAX);
	if (!dir)
		return (NULL);
	if (getcwd(dir, PATH_MAX) == NULL)
	{
		free(dir);
		perror("minishell: pwd: getcwd");//bash ile hata kodunu kontorl et
		return (NULL);
	}
	printf("%s\n", dir);
	free(dir);
}

void	split_to_list(t_pwd	*list, char *pwd)
{
	t_pwd *tmp;
	char	**split;
	
	split = ft_split(pwd, '\\');
	if (!split)
		return (NULL);
	
}

char	*logical_path(t_sh *sh, t_env *pwd, char *path)
{
	t_pwd	*pre;
	char	**curr;

	if (path && path[0] == '\\')
		return (path);
	pre = malloc(sizeof(t_pwd));
	if (!pre)
		return (NULL);
	curr = ft_split(path, '\\');
	if (!curr)
		return (NULL);
	split_to_list(&pre, pwd->env_value);
}

void	update_pwd(t_sh	*sh, t_env *oldpwd, t_env *pwd, char *path)
{
	char	*new_pwd;
	char	*new_path;
	t_gc	*cp;

	cp = gc_checkpoint(sh);
	new_pwd = gc_malloc(sh, PATH_MAX, 1);
	if (!new_pwd)
		return ;
	if (getcwd(new_pwd, PATH_MAX))
	{
		free(oldpwd->env_value);
		oldpwd->env_value = pwd->env_value;
		pwd->env_value = new_pwd;	
	}
	else
		new_path = logical_path(sh, pwd, path);
	gc_rollback(sh, cp);
}

void	builtin_cd(t_sh *sh, char *path)
{
	t_env	*tmp;
	t_env	*pwd_adr;
	t_env	*oldpwd_adr;

	tmp = sh->env;
	if (!path)
		return ;
	if (chdir(path) < 0)
	{
		perror("");
		return ;
	}
	while (tmp)
	{
		if (!ft_strncmp(tmp->env_name, "PWD", 3))
			pwd_adr = tmp;
		else if (!ft_strncmp(tmp->env_name, "OLDPWD", 7))
			oldpwd_adr = tmp;
		tmp = tmp->next;
	}
	update_pwd(sh, oldpwd_adr, pwd_adr, path);
}

	// free(oldpwd_adr->env_value);
	// oldpwd_adr->env_value = pwd_adr->env_value;
	// pwd_adr->env_value = builtin_pwd(sh);



