#include "WCMS.h"

void	update_oldpwd(t_sh *sh)
{
	char	*to_oldpwd;
	to_oldpwd = get_env_value(sh, "PWD");
	if (to_oldpwd)
}

void	builtin_cd(t_sh	*sh, char **param)
{
	update_oldpwd(sh);
}
