#include "WCMS.h"

void	allocate_error(t_sh *sh)
{
	sh->exit_status = 1;
	ft_putendl_fd("minishell: cannot allocate memory", 2);
}

void	no_home_error(t_sh *sh)
{
	ft_putendl_fd("minishell: cd: HOME not set", 2);
	sh->exit_status = 1;
	return ;
}

