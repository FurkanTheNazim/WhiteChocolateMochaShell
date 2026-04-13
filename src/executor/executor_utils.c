/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 06:09:45 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/14 00:17:05 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->env_name && env->has_value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*join_env_entry(t_sh *sh, char *name, char *value)
{
	char	*tmp;
	char	*result;

	tmp = gc_add(sh, ft_strjoin(name, "="), 0);
	if (!tmp)
		return (NULL);
	result = gc_add(sh, ft_strjoin(tmp, value), 0);
	return (result);
}

char	**env_to_envp(t_sh *sh)
{
	char	**envp;
	t_env	*tmp;
	int		i;
	int		size;

	size = env_list_size(sh->env);
	envp = gc_malloc(sh, sizeof(char *) * (size + 1), 0);
	if (!envp)
		return (NULL);
	tmp = sh->env;
	i = 0;
	while (tmp)
	{
		if (tmp->env_name && tmp->has_value)
		{
			envp[i] = join_env_entry(sh, tmp->env_name, tmp->env_value);
			if (!envp[i])
				return (NULL);
			i++;
		}
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

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

char	*resolve_path(t_sh *sh, char *cmd_name)
{
	t_env	*path_env;
	char	**dirs;
	char	*result;
	int		i;

	if (!cmd_name || !cmd_name[0])
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (cmd_name);
		return (NULL);
	}
	path_env = find_env(sh, "PATH");
	if (!path_env || !path_env->env_value)
		return (NULL);
	dirs = gc_add(sh, ft_split(path_env->env_value, ':'), 0);
	if (!dirs)
		return (NULL);
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
