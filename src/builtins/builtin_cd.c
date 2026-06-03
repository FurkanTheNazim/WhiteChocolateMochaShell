/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 15:44:22 by kedemiro          #+#    #+#             */
/*   Updated: 2026/06/03 18:41:20 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"
#include <errno.h>

static char	*get_env_home_value(t_sh *sh, char *str, int *finded)
{
	t_env	*tmp;
	size_t	len;
	char	*result;

	if (!str || !str[0])
		return (NULL);
	len = ft_strlen(str);
	tmp = sh->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->env_name, str, len + 1))
		{
			if (!tmp->env_value)
				return ("");
			result = gc_add(sh, ft_strdup(tmp->env_value), 0);
			if (!result)
				return (NULL);
			*finded = 1;
			return (result);
		}
		tmp = tmp->next;
	}
	return ("");
}

int	cd_home(t_sh *sh, char **param)
{
	char	*home;
	int		finded;

	finded = 0;
	home = get_env_home_value(sh, "HOME", &finded);
	if (!home)
		return (allocate_error(sh), -1);
	else if (!home[0])
	{
		if (finded == 0)
			return (no_home_error(sh), -1);
		else
			return (1);
	}
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
	if (param[1] && param[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		sh->exit_status = 1;
		return ;
	}
	if (!param[1])
	{
		if (cd_home(sh, &param[1]) < 0)
			return ;
	}
	else if (param[1] && !param[1][0])
	{
		sh->exit_status = 0;
		return ;
	}
	else
	{
		if (chdir(param[1]) < 0)
			cd_error(sh, param[1], errno);
	}
	update_pwds(sh, param[1]);
}
