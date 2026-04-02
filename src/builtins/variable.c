/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:46:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/02 18:03:12 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

int	is_assignment(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] != '=')
		return (0);
	return (1);
}

static int	update_var(t_sh *sh, char **nv)
{
	t_env	*tmp;
	size_t	len;

	tmp = sh->env;
	len = ft_strlen(nv[0]);
	while (tmp)
	{
		if (ft_strlen(tmp->env_name) == len
			&& ft_strncmp(tmp->env_name, nv[0], len) == 0)
		{
			if (nv[1])
			{
				tmp->env_value = nv[1];
				tmp->has_value = 1;
			}
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	exec_assignment(t_sh *sh, char *str)
{
	char	**nv;
	t_env	*node;

	nv = get_name_value(sh, str);
	if (!nv)
		return ;
	if (!update_var(sh, nv))
	{
		node = env_newnode(sh, nv[0], nv[1]);
		if (!node)
			return ;
		node->exported = 0;
		env_addback(sh, node);
	}
}

int	handle_assignments(t_sh *sh, t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	i = 0;
	while (cmd->args[i])
	{
		if (!is_assignment(cmd->args[i]))
			return (0);
		i++;
	}
	i = 0;
	while (cmd->args[i])
	{
		exec_assignment(sh, cmd->args[i]);
		i++;
	}
	return (1);
}
