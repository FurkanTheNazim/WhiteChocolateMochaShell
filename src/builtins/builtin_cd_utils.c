/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:15:05 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/12 20:16:42 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

t_env	*find_env(t_sh	*sh, char *name)
{
	t_env	*tmp;
	size_t	len;

	if (!sh || !name)
		return (NULL);
	len = ft_strlen(name);
	tmp = sh->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->env_name, name, len) && tmp->env_name[len] == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	count_max_lvl(char **base, char **path)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (base[i])
		i++;
	while (path[j])
	{
		if (!ft_strncmp(path[j], "..", 2) && !path[j][2])
		{
			if (i != 0)
				i--;
		}
		else if (!ft_strncmp(path[j], ".", 1) && !path[j][1]);
		else
			i++;
		j++;	
	}
	return (i);
}

void	update_env_value(t_sh *sh, char *name, char *new_value, int has_value)
{
	t_env	*tmp;

	if (!name)
		return ;
	else if  (!new_value && has_value)
		return ;
	tmp = find_env(sh, name);
	if (!tmp)
		return ;
	gc_free(sh, tmp->env_value, 1);
	if (!new_value && !has_value)
	{
		tmp->env_value = NULL;
		return ;	
	}
	tmp->env_value = gc_add(sh, new_value, 1);
	if (!tmp->env_value)
		return (allocate_error(sh));
	tmp->has_value = has_value;
}

void	create_oldpwd(t_sh *sh, t_env *new_oldpwd)
{
	t_env	*tmp;

	if (new_oldpwd)
	{
		tmp = env_newnode(sh, "OLDPWD", (new_oldpwd)->env_value);
		tmp->env_printable = 0;
		env_addback(sh, tmp);
	}
}
