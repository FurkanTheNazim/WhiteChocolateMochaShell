/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   builtin_export.c                                     :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/12 20:11:15 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 21:09:40 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

int	validate_param(t_sh *sh, char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (export_err(sh, str), 0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (export_err(sh, str), 0);
		i++;
	}
	return (1);
}

char	**get_name_value(t_sh *sh, char *str)
{
	char	**ret;
	int		i;

	i = 0;
	ret = gc_malloc(sh, sizeof(char *) * 3, 1);
	if (!ret)
	{
		ft_putendl_fd("minishell: cannot allocate memory", 2);
		sh->exit_status = 1;
		return (NULL);
	}
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '\0')
	{
		ret[0] = gc_add(sh, ft_strdup(str), 1);
		ret[1] = NULL;
	}
	else
	{
		ret[0] = gc_add(sh, ft_substr(str, 0, i), 1);
		ret[1] = gc_add(sh, ft_substr(str, i + 1, ft_strlen(str + i + 1)), 1);
	}
	ret[2] = NULL;
	return (ret);
}

int	check_list(t_sh *sh, char **param)
{
	t_env	*tmp;

	tmp = sh->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_name, param[0], ft_strlen(param[0])) == 0
			&& !tmp->env_name[ft_strlen(param[0])])
		{
			tmp->env_name = param[0];
			if (param[1])
			{
				tmp->env_value = param[1];
				tmp->has_value = 1;
			}
			tmp->exported = 1;
			tmp->env_printable = 1;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	builtin_export(t_sh *sh, char **param)
{
	char	**name_value;
	int		i;

	i = 1;
	if (param && !param[1])
		declare(sh);
	else
	{
		while (param[i])
		{
			if (validate_param(sh, param[i]))
			{
				name_value = get_name_value(sh, param[i]);
				if (!name_value)
					return ;
				if (!check_list(sh, name_value))
					env_addback(sh, env_newnode(sh, name_value[0],
							name_value[1]));
			}
			i++;
		}
	}
}
