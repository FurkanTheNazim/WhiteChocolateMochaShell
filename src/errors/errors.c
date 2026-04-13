/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:12:00 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/13 12:44:25 by kedemiro         ###   ########.fr       */
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
