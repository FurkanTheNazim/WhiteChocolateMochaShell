/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static char	*try_path(t_sh *sh, char *dir, char *cmd_name)
{
	char	*tmp;
	char	*full_path;

	tmp = gc_add(sh, ft_strjoin(dir, "/"), 0);
	if (!tmp)
		return (NULL);
	full_path = gc_add(sh, ft_strjoin(tmp, cmd_name), 0);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	return (NULL);
}

static char	*check_absolute_path(char *cmd_name)
{
	if (access(cmd_name, X_OK) == 0)
		return (cmd_name);
	return (NULL);
}

char	*resolve_path(t_sh *sh, char *cmd_name)
{
	t_env	*path_env;
	char	**dirs;
	char	*result;
	int		i;

	if (!cmd_name || !cmd_name[0])
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
		return (check_absolute_path(cmd_name));
	path_env = find_env(sh, "PATH");
	if (!path_env || !path_env->env_value)
		return (NULL);
	dirs = gc_add_matrix(sh, (void **)ft_split(path_env->env_value, ':'), 0);
	i = 0;
	while (dirs && dirs[i])
	{
		result = try_path(sh, dirs[i], cmd_name);
		if (result)
			return (result);
		i++;
	}
	return (NULL);
}
