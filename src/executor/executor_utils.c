/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 06:09:45 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/14 16:15:54 by kedemiro         ###   ########.fr       */
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

void	handle_cmd_not_found(t_command *cmd)
{
	char	*msg;
	char	*tmp;

	msg = ft_strjoin("minishell: ", cmd->args[0]);
	if (ft_strchr(cmd->args[0], '/') && access(cmd->args[0], F_OK) == 0)
	{
		tmp = ft_strjoin(msg, ": Permission denied\n");
		ft_putstr_fd(tmp, 2);
		free(msg);
		free(tmp);
		exit(126);
	}
	tmp = ft_strjoin(msg, ": command not found\n");
	ft_putstr_fd(tmp, 2);
	free(msg);
	free(tmp);
	exit(127);
}

void	wait_single_child(t_sh *sh, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->exit_status = WTERMSIG(status) + 128;
}
