/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:10:15 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/17 18:18:56 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"
#include <errno.h>

int	cd_home(t_sh *sh, char **param)
{
	char	*home;

	home = get_env_value(sh, "HOME");
	if (!home)
		return (allocate_error(sh), -1);
	else if (!home[0])
		return (no_home_error(sh), -1);
	if (chdir(home) < 0)
	{
		perror("minishell: cd: ");
		sh->exit_status = 1;
		return (-1);
	}
	*param = gc_add(sh, ft_strdup(home), 0);
	if (!*param)
		return (allocate_error(sh), -1);
	return (1);
}

void	update_pwds(t_sh *sh, char *param)
{
	t_env	*env_pwd;
	t_env	*env_oldpwd;
	char	*curr_pwd;

	env_pwd = find_env(sh, "PWD");
	env_oldpwd = find_env(sh, "OLDPWD");
	if (!env_oldpwd)
		create_oldpwd(sh, env_pwd);
	curr_pwd = getcwd(NULL, 0);
	if (!curr_pwd)
		return (logical_pwd_update(sh, param, env_pwd));
	if (!env_pwd)
	{
		env_pwd = env_newnode(sh, "PWD", "");
		env_pwd->env_printable = 0;
		env_addback(sh, env_pwd);
	}
	else if (env_pwd->has_value)
	{
		env_oldpwd->env_value = gc_add(sh, ft_strdup(env_pwd->env_value), 1);
		env_oldpwd->has_value = 1;
	}
	env_pwd->env_value = gc_add(sh, ft_strdup(curr_pwd), 1);
	env_pwd->has_value = 1;
	return (free(curr_pwd));
}

void	builtin_cd(t_sh *sh, char **param)
{
	if (!param[1])
	{
		if (cd_home(sh, &param[1]) < 0)
			return ;
	}
	else if (param[1] && !param[1][0])
	{
		// ft_putendl_fd("minishell: cd: null directory", 2);//expander world split yüzünden NAME="" cd $NAME durumunda bu hata değil errno gelmeli, cd "$NAME" world splitter a girmeyeceği için bu hata gelmeli, kontrol et sonradan //kampüste test et
		sh->exit_status = 0;
		return ;
	}
	else
	{
		if (chdir(param[1]) < 0)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(param[1], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2); //errno kullanımı ile ilgili araştır bence kullanılabilmeli çünkü strerror için errno şart ve izin erilen fonksiyonlar arasında
			sh->exit_status = 1;
			return ;
		}
	}
	update_pwds(sh, param[1]);
}
