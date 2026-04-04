/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 18:09:35 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/04 18:56:56 by mahmmous         ###   ########.fr       */
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

int permanent_update(t_sh *sh, char *str)
{
	char	**nv;
	t_env	*node;

	nv = env_delimeter(sh, str, 0);
	if(!nv)
		return (-1);
	node = find_env(sh, nv[0]);
	if(node)
		node->env_value = nv[1];
	else
	{
		node = env_newnode(sh, nv[0], nv[1]);
		if(!node)
			return (allocate_error(sh), -1);
		node->exported = 0;
		node->env_printable = 0;
		if(!env_addback(sh, node))
			return (allocate_error(sh), -1);
	}
	return (1);
}

int	exec_assignment(t_sh *sh, char *str)
{
	char	**nv;
	t_env	*node;


	nv = var_delimeter(sh, str, 0);
	if (!nv)
		return (-1);
	node = find_env(sh, nv[0]);
	if(node)
	{
		node->temp_flag = 1;
		node->old_value = node->env_value;
		node->env_value = nv[1];
	}
	else 
	{
		if(!env_addback(sh, env_newnode(sh, nv[0], nv[1])))
			return (allocate_error(sh), -1);
	}
	return (1);
}

int	handle_assignments(t_sh *sh, t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	i = 0;
	while (cmd->args[i])
	{
		if (!is_assignment(cmd->args[i]) && i == 0)
			return (0);
		if (!cmd->args[1])
		{
			if(permanent_update(sh, cmd->args[i]) == -1)
				return (-1);
		}
		else if(exec_assignment(sh, cmd->args[i]) == -1)
			return (-1);
		i++;
	}
	return (1);
}