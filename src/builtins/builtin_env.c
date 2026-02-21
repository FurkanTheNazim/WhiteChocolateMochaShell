#include "WCMS.h"

void	builtin_env(t_sh *sh, char *param)
{
	t_env	*tmp;
	(void)param;

	tmp = sh->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_name, "PATH", 4) == 0 && sh->default_path)
			tmp = tmp->next;
		if (tmp && tmp->only_export == 0)
		{
			printf("%s=%s\n", tmp->env_name, tmp->env_value);
			tmp = tmp->next;
		}
	}
}