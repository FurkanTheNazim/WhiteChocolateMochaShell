/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   enviroment_utils.c                                   :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/12 20:11:49 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:48:39 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

t_env	*env_newnode(t_sh *sh, char *name, char *value)
{
	t_env	*node;

	if (!name)
		return (NULL);
	node = gc_malloc(sh, sizeof(t_env), 1);
	if (!node)
		return (NULL);
	node->env_name = name;
	node->env_value = value;
	node->exported = 1;
	if (value)
		node->has_value = 1;
	else
		node->has_value = 0;
	node->env_printable = 1;
	node->temp_flag = 0;
	node->old_value = NULL;
	node->next = NULL;
	return (node);
}

int	env_addback(t_sh *sh, t_env *newnode)
{
	t_env	*tmp;

	if (!newnode)
		return (-1);
	if (!sh->env)
	{
		sh->env = newnode;
		return (1);
	}
	tmp = sh->env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = newnode;
	return (1);
}

void	update_lastcmd_env(t_sh *sh, t_command *cmd, char *path, int flag)
{
	t_env	*last_cmd;

	last_cmd = find_env(sh, "_");
	if (flag)
	{
		flag = 0;
		if (last_cmd)
		{
			while (cmd->args[flag])
				flag++;
			last_cmd->env_value = gc_add(sh, ft_strdup(cmd->args[flag -1]), 1);
		}
	}
	else
	{
		if (last_cmd && path)
			last_cmd->env_value = gc_add(sh, ft_strdup(path), 1);
	}
}

void	normalize_env(t_sh *sh)
{
	t_env	*tmp;

	tmp = sh->env;
	while (tmp)
	{
		if (tmp->temp_flag)
		{
			tmp->env_value = tmp->old_value;
			tmp->temp_flag = 0;
		}
		tmp = tmp->next;
	}
}
