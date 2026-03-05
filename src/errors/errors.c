#include "WCMS.h"

void	allocate_error(t_sh *sh)
{
	sh->exit_status = 1;
	ft_putendl_fd("minishell: cannot allocate memory", 2);
}

