/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:12:00 by kedemiro          #+#    #+#             */
/*   Updated: 2026/06/03 18:41:07 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	exit_err(t_sh *sh, char *err, char *param, int stat)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (param)
	{
		ft_putstr_fd(param, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(err, 2);
	sh->exit_status = stat;
	return ;
}

void	env_error(t_sh *sh, char *err)
{
	ft_putendl_fd(err, 2);
	gc_free_all(sh);
	exit(1);
}

void	cd_error(t_sh *sh, char *param, int error)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(param, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(error), 2);
	sh->exit_status = 1;
	return ;
}
