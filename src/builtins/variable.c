/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   variable.c                                           :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/04 18:09:35 by mahmmous            #+#    #+#           */
/*   Updated: 2026/05/28 19:23:55 by kedemiro           ###   ########.fr     */
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

int	permanent_update(t_sh *sh, char *str)
{
	char	**nv;
	t_env	*node;

	nv = env_delimeter(sh, str, 0);
	if (!nv)
		return (-1);
	node = find_env(sh, nv[0]);
	if (node)
		node->env_value = nv[1];
	else
	{
		node = env_newnode(sh, nv[0], nv[1]);
		if (!node)
			return (allocate_error(sh), -1);
		node->exported = 0;
		node->env_printable = 0;
		if (!env_addback(sh, node))
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
	if (node)
	{
		node->temp_flag = 1;
		node->old_value = node->env_value;
		node->env_value = nv[1];
	}
	else
	{
		if (!env_addback(sh, env_newnode(sh, nv[0], nv[1])))
			return (allocate_error(sh), -1);
	}
	return (1);
}

int	handle_assignments(t_sh *sh, t_command *cmd)
{
	int	i;
	int	has_command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	i = 0;
	while (cmd->args[i] && is_assignment(cmd->args[i]))
		i++;
	if (i == 0)
		return (0);
	has_command = (cmd->args[i] != NULL);
	i = 0;
	while (cmd->args[i] && is_assignment(cmd->args[i]))
	{
		if (!has_command)
		{
			if (permanent_update(sh, cmd->args[i]) == -1)
				return (-1);
		}
		else if (exec_assignment(sh, cmd->args[i]) == -1)
			return (-1);
		i++;
	}
	cmd->args = &(cmd->args[i]);
	return (1);
}
