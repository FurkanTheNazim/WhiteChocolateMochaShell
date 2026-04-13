/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:11:22 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/12 20:11:23 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	env_del_node(t_sh *sh, t_env *list)
{
	t_env	*curr;
	t_env	*prev;

	if (!list)
		return ;
	curr = sh->env;
	prev = NULL;
	while (curr)
	{
		if (curr == list)
		{
			if (prev)
				prev->next = curr->next;
			else
				sh->env = curr->next;
			gc_free(sh, curr, 1);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	builtin_unset(t_sh *sh, char **param)
{
	int		i;
	t_env	*tmp;
	t_env	*next;

	i = 1;
	while (param[i])
	{
		tmp = sh->env;
		while (tmp)
		{
			next = tmp->next;
			if (ft_strlen(param[i]) == ft_strlen(tmp->env_name)
				&& ft_strncmp(tmp->env_name, param[i], ft_strlen(param[i])) == 0)
			{
				env_del_node(sh, tmp);
				break ;
			}
			tmp = next;
		}
		i++;
	}
	sh->exit_status = 0;
}
