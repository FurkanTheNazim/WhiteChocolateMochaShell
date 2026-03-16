#include "WCMS.h"

void	cd_home(t_sh *sh)
{
	char	*home;

	home = get_env_value(sh, "HOME");
	if (!home)
		return (allocate_error(sh));
	else if (!home[0])
		return (no_home_error(sh));
	if (chdir(home) < 0)
	{
		perror("minishell: cd: ");
		sh->exit_status = 1;
	}
	return ;
}

void	builtin_cd(t_sh *sh, char **param)
{
	if (!param[1])
	{
		cd_home(sh);
		return ;	
	}
	if (param[1] && !param[1][0])
	{
		ft_putendl_fd("minishell: cd: null directory", 2);//expander world split yüzünden NAME="" cd $NAME durumunda bu hata değil errno gelmeli, cd "$NAME" world splitter a girmeyeceği için bu hata gelmeli, kontrol et sonradan
		sh->exit_status = 1;
		return ;
	}
}