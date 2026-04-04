#include "WCMS.h"

char	**var_delimeter(t_sh *sh, char *envp, int i)
{
	char	**ret;

	ret = gc_malloc(sh, (sizeof(char *) * 3), 1);
	if (!ret)
		env_error(sh, "minishell: enviroment error\n");
	while (envp[i])
	{
		if (envp[i] == '=')
			break ;
		i++;
	}
	if (envp[i] == '\0')
	{
		ret[0] = gc_add(sh, ft_strdup(envp), 0);
		ret[1] = gc_add(sh, ft_strdup(""), 0);
	}
	else
	{
		ret[0] = gc_add(sh, ft_substr(envp, 0, i), 0);
		ret[1] = gc_add(sh, ft_substr(envp, i +1, ft_strlen(envp +i +1)), 0);
	}
	ret[2] = NULL;
	return (ret);
}