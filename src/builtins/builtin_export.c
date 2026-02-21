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

void	builtin_export(t_sh *sh, char *param)
{
	
	
}
