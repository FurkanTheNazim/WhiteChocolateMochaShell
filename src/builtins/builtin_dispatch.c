/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:47:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/03/19 16:47:00 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	identify_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (NOT_BUILTIN);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (BUILTIN_UNSET);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BUILTIN_EXIT);
	return (NOT_BUILTIN);
}

int	exec_builtin(t_sh *sh, t_command *cmd)
{
	if (!cmd || cmd->builtin == NOT_BUILTIN)
		return (0);
	if (cmd->builtin == BUILTIN_ECHO)
		builtin_echo(sh, cmd->args);
	else if (cmd->builtin == BUILTIN_CD)
		builtin_cd(sh, cmd->args);
	else if (cmd->builtin == BUILTIN_PWD)
		builtin_pwd(sh, cmd->args);
	else if (cmd->builtin == BUILTIN_EXPORT)
		builtin_export(sh, cmd->args);
	else if (cmd->builtin == BUILTIN_UNSET)
		builtin_unset(sh, cmd->args);
	else if (cmd->builtin == BUILTIN_ENV)
		builtin_env(sh, cmd->args);
	else if (cmd->builtin == BUILTIN_EXIT)
		builtin_exit(sh, cmd->args);
	return (1);
}
