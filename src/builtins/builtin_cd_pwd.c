/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   builtin_cd_pwd.c                                     :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/12 20:18:32 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:20:03 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	absolute_pwd_update(t_sh *sh, t_env **curr_pwd, char *param)
{
	if (*curr_pwd)
	{
		gc_free(sh, (*curr_pwd)->env_value, 1);
		(*curr_pwd)->env_value = gc_add(sh, ft_strdup(param), 1);
		return ;
	}
	else
	{
		(*curr_pwd) = env_newnode(sh, "PWD", param);
		if (!*curr_pwd)
			return (allocate_error(sh));
		(*curr_pwd)->exported = 0;
		(*curr_pwd)->env_printable = 0;
		env_addback(sh, *curr_pwd);
	}
}

char	*build_path(t_sh *sh, char **base, int b_lvl)
{
	char	*result;
	char	*tmp;
	int		i;

	if (b_lvl == 0)
		return (gc_add(sh, ft_strdup("/"), 1));
	result = gc_add(sh, ft_strdup(""), 0);
	i = 0;
	while (i < b_lvl)
	{
		tmp = gc_add(sh, gc_join("/", base[i]), 0);
		result = gc_add(sh, gc_join(result, tmp), 0);
		if (!result)
			return (allocate_error(sh), NULL);
		i++;
	}
	return (result);
}

void	calculate_new_pwd(t_sh	*sh, char ***result, char **base, char **path)
{
	int	all_size;
	int	p_b_lvl[2];

	all_size = count_max_lvl(base, path);
	*result = gc_add(sh, ft_calloc(all_size + 1, sizeof(char *)), 0);
	ft_bzero(p_b_lvl, sizeof(p_b_lvl));
	while (base[p_b_lvl[1]])
	{
		(*result)[p_b_lvl[1]] = base[p_b_lvl[1]];
		p_b_lvl[1]++;
	}
	while (path[p_b_lvl[0]])
	{
		if (!ft_strncmp(path[p_b_lvl[0]], "..", 2) && !path[p_b_lvl[0]][2])
		{
			if (p_b_lvl[1] > 0)
				p_b_lvl[1]--;
		}
		else if (!ft_strncmp(path[p_b_lvl[0]], ".", 1) && !path[p_b_lvl[0]][1])
			;
		else
			(*result)[p_b_lvl[1]++] = path[p_b_lvl[0]];
		p_b_lvl[0]++;
	}
}

void	create_pwd(t_sh *sh, char *param, t_env *new_oldpwd)
{
	char	**path;
	char	**base;
	char	**result;

	base = gc_add_matrix(sh, (void **)ft_split(new_oldpwd->env_value, '/'), 0);
	path = gc_add_matrix(sh, (void **)ft_split(param, '/'), 0);
	if (!base || !path)
		return (allocate_error(sh));
	calculate_new_pwd(sh, &result, base, path);
	update_env_value(sh, "PWD",
		build_path(sh, result, count_max_lvl(base, path)), 1);
	gc_free(sh, base, 1);
	gc_free(sh, path, 1);
}

void	logical_pwd_update(t_sh *sh, char *param, t_env *new_oldpwd)
{
	t_env	*curr_pwd;

	curr_pwd = find_env(sh, "PWD");
	if (param[0] == '/')
		absolute_pwd_update(sh, &curr_pwd, param);
	else
	{
		if (!new_oldpwd)
			return ;
		create_pwd(sh, param, new_oldpwd);
	}
}
