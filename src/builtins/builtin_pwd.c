#include "../../includes/WCMS.h"

void    builtin_pwd(t_sh *sh, char **param)
{
	char	*pwd;

	(void)param;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		pwd = get_env_value(sh, "PWD");
		if (!pwd)
		{
			ft_putendl_fd("minishell: pwd: error retrieving current directory", 2);
			sh->exit_status = 1;
			return ;
		}
		ft_printf("%s\n", pwd);
		sh->exit_status = 1;
		return ;
	}
	ft_printf("%s\n", pwd);
	free(pwd);
	sh->exit_status = 0;
}