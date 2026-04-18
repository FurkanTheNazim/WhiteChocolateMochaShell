/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:11:03 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/18 18:03:08 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	builtin_env(t_sh *sh, char **param)
{
	t_env	*tmp;
	(void)param;

	tmp = sh->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_name, "PATH", 4) == 0 && sh->default_path)
			tmp = tmp->next;
		else if (tmp->exported && tmp->has_value && tmp->env_printable)
		{
			printf("%s=%s\n", tmp->env_name, tmp->env_value);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}
