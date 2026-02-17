#include "WCMS.h"
#include "builtin.h"

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

int validate_param(char *str)
{
    int i = 0;
    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

void	add_back(t_sh	*sh, );

void	builtin_export(t_sh *sh, char *param)
{
	t_env	*env;
	t_env	*node;
	char	*tmp;

	env = sh->env;
	if (!validate_param(param))
		return (perror(EXPORT_ERR));
	tmp = ft_strchr(param, '=');
	if (tmp)
	{
		node->env_value = ft_substr((tmp + 1), 0, ft_strlen(tmp + 1));
		if (!node->env_value)
		{
			sh->exit_status = 1;
			return ;
		}
		node->env_name = ft_substr(param, 0, tmp - param);
		if (!node->env_name)
		{
			sh->exit_status = 1;
			return ;
		}
		gc_add(sh, node->env_name);
		gc_add(sh, node->env_value);
	}
	else

}
