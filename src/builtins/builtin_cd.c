#include "../../includes/WCMS.h"
#include <errno.h>

int	cd_home(t_sh *sh, char **param)
{
	char	*home;

	home = get_env_value(sh, "HOME");
	if (!home)
		return (allocate_error(sh), -1);
	else if (!home[0])
		return (no_home_error(sh), -1);
	if (chdir(home) < 0)
	{
		perror("minishell: cd: ");
		sh->exit_status = 1;
		return (-1);
	}
	*param = gc_add(sh, ft_strdup(home), 0);
	if (!*param)
		return (allocate_error(sh), -1);
	return 1;
}

t_env	*find_env(t_sh	*sh, char *name)
{
	t_env	*tmp;
	size_t	len;

	if (!sh || !name)
		return (NULL);
	len = ft_strlen(name);
	tmp = sh->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->env_name, name, len) && tmp->env_name[len] == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	absolute_pwd_update(t_sh *sh, t_env **curr_pwd, char *param)
{
	if (*curr_pwd)
	{
		gc_free(sh, (*curr_pwd)->env_value, 1);
		(*curr_pwd)->env_value = gc_add(sh, ft_strdup(param), 1);
		return ;
	}
	else
	{
		(*curr_pwd) = env_newnode(sh, "PWD", param);
		if (!*curr_pwd)
			return (allocate_error(sh));
		(*curr_pwd)->exported = 0;
		(*curr_pwd)->env_printable = 0;
		env_addback(sh, *curr_pwd);
	}
}

int	count_max_lvl(char **base, char **path)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (base[i])
		i++;
	while (path[j])
	{
		if (!ft_strncmp(path[j], "..", 2) && !path[j][2])
		{
			if (i != 0)
				i--;
		}
		else if (!ft_strncmp(path[j], ".", 1) && !path[j][1]);
		else
			i++;
		j++;	
	}
	return (i);
}

char	*build_path(t_sh *sh, char **base, int b_lvl)
{
	char	*result;
	char	*tmp;
	int		i;

	if (b_lvl == 0)
		return (gc_add(sh, ft_strdup("/"), 1));
	result = gc_add(sh, ft_strdup(""), 0);
	i = 0;
	while (i < b_lvl)
	{
		tmp = gc_add(sh, gc_join("/", base[i]), 0);
		result = gc_add(sh, gc_join(result, tmp), 0);
		if (!result)
			return (allocate_error(sh), NULL);
		i++;
	}
	return (result);
}

void	update_env_value(t_sh *sh, char *name, char *new_value, int has_value)
{
	t_env	*tmp;

	if (!name)
		return ;
	else if  (!new_value && has_value)
		return ;
	tmp = find_env(sh, name);
	if (!tmp)
		return ;
	gc_free(sh, tmp->env_value, 1);
	if (!new_value && !has_value)
	{
		tmp->env_value = NULL;
		return ;	
	}
	tmp->env_value = gc_add(sh, new_value, 1);
	if (!tmp->env_value)
		return (allocate_error(sh));
	tmp->has_value = has_value;
}

void	calculate_new_pwd(t_sh	*sh, char ***result, char **base, char **path)
{
	int	all_size;
	int	b_lvl;
	int	p_lvl;

	all_size = count_max_lvl(base, path);
	*result = gc_add(sh, ft_calloc(all_size + 1, sizeof(char *)), 0);
	b_lvl = 0;
	p_lvl = 0;
	while (base[b_lvl])
	{
		(*result)[b_lvl] = base[b_lvl];
		b_lvl++;
	}
	while (path[p_lvl])
	{
		if (!ft_strncmp(path[p_lvl], "..", 2) && !path[p_lvl][2])
		{
			if (b_lvl > 0)
				b_lvl--;
		}
		else if (!ft_strncmp(path[p_lvl], ".", 1) && !path[p_lvl][1]);
		else
			(*result)[b_lvl++] = path[p_lvl];
		p_lvl++;
	}
}

void	create_pwd(t_sh *sh, char *param, t_env *new_oldpwd)
{
	char	**path;
	char	**base;
	char	**result;

	base = gc_add_matrix(sh, (void **)ft_split(new_oldpwd->env_value, '/'), 0);
	path = gc_add_matrix(sh, (void **)ft_split(param, '/'), 0);
	if (!base || !path)
		return (allocate_error(sh));
	calculate_new_pwd(sh, &result, base, path);
	update_env_value(sh, "PWD", build_path(sh, result, count_max_lvl(base, path)), 1);
	gc_free(sh, base, 1);
	gc_free(sh, path, 1);
}

void	logical_pwd_update(t_sh *sh, char *param, t_env *new_oldpwd)
{
	t_env	*curr_pwd;

	curr_pwd = find_env(sh, "PWD");
	if (param[0] == '/')
		absolute_pwd_update(sh, &curr_pwd, param);
	else
	{
		if (!new_oldpwd)
			return ;
		create_pwd(sh, param, new_oldpwd);
	}
}

void	create_oldpwd(t_sh *sh, t_env *new_oldpwd)
{
	t_env	*tmp;

	if (new_oldpwd)
	{
		tmp = env_newnode(sh, "OLDPWD", (new_oldpwd)->env_value);
		tmp->env_printable = 0;
		env_addback(sh, tmp);
	}
}

void	update_pwds(t_sh *sh, char *param)
{
	t_env	*env_pwd;
	t_env	*env_oldpwd;
	char	*curr_pwd;

	env_pwd = find_env(sh, "PWD");
	env_oldpwd = find_env(sh, "OLDPWD");
	if (!env_oldpwd)
		create_oldpwd(sh, env_pwd);
	curr_pwd = getcwd(NULL, 0);
	if (!curr_pwd)
		return (logical_pwd_update(sh, param, env_pwd));
	if (!env_pwd)
	{
		env_pwd = env_newnode(sh, "PWD", "");
		env_pwd->env_printable = 0;
		env_addback(sh, env_pwd);
	}
	else if (env_pwd->has_value)
	{
		env_oldpwd->env_value = gc_add(sh, ft_strdup(env_pwd->env_value), 1);
		env_oldpwd->has_value = 1;
	}
	env_pwd->env_value = gc_add(sh, ft_strdup(curr_pwd), 1);
	env_pwd->has_value = 1;
	return (free(curr_pwd));
}

void	builtin_cd(t_sh *sh, char **param)
{
	if (!param[1])
	{
		if (cd_home(sh, &param[1]) < 0)
			return ;
	}
	else if (param[1] && !param[1][0])
	{
		ft_putstr_fd("minishell: cd: null directory", 2);//expander world split yüzünden NAME="" cd $NAME durumunda bu hata değil errno gelmeli, cd "$NAME" world splitter a girmeyeceği için bu hata gelmeli, kontrol et sonradan
		sh->exit_status = 1;
		return ;
	}
	else
	{
		if (chdir(param[1]) < 0)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(param[1], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2); //errno kullanımı ile ilgili araştır bence kullanılabilmeli çünkü strerror için errno şart ve izin erilen fonksiyonlar arasında
			sh->exit_status = 1;
			return ;
		}
	}
	update_pwds(sh, param[1]);
}
