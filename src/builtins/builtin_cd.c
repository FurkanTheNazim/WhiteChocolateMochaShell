#include "WCMS.h"
#include "builtin.h"


void	update_pwd(t_sh *sh, t_cd *data, char *path)
{
	char	*newpwd;

	newpwd = malloc(sizeof(char) * MAX_PATH + 1);
	if (!newpwd)
		return ;
	if (!(data->pwd_adr) && !(data->tmp_pwd_f))
	{
		
	}

}

t_env	*find_env_adr(t_sh *sh, char *env_name)
{
	t_env	*tmp;
	int		len;

	len = ft_strlen(env_name);
	tmp = sh->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_name, env_name, len) == 0
			&& ((tmp->env_name)[len] == '=' || !(tmp->env_name)[len]))
			return (tmp);
		tmp = tmp->next;	
	}
	return (NULL);
}

void	builtin_cd(t_sh	*sh, char path)
{
	t_cd	data;
	data.tmp_pwd_f = 0;

	if (!path)
		return ;
	if (!getcwd(data.tmp_pwd, MAX_PATH))
		data.tmp_pwd_f = 1;
	else
		data.tmp_pwd[0] = '\0';
	if (chdir(path) < 0)
		return (perror(""));
	data.oldpwd_adr = find_env_adr(sh, "OLDPWD");
	data.pwd_adr = find_env_adr(sh, "PWD");
	update_pwd(sh, &data, path);
}
