/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   builtin_export_declare.c                             :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 19:28:54 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:33:47 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

int	env_addcopy(t_env **copy, t_env *tmp)
{
	t_env	*tmp_copy;

	if (!tmp)
		return (-1);
	if (!*copy)
	{
		*copy = tmp;
		return (1);
	}
	tmp_copy = *copy;
	while (tmp_copy->next)
		tmp_copy = tmp_copy->next;
	tmp_copy->next = tmp;
	return (1);
}

t_env	*copy_env(t_sh *sh)
{
	t_env	*copy;
	t_env	*tmp;
	t_env	*origin;

	origin = sh->env;
	copy = NULL;
	while (origin)
	{
		tmp = gc_malloc(sh, sizeof(t_env), 0);
		if (!tmp)
			return (allocate_error(sh), NULL);
		tmp->env_name = gc_add(sh, ft_strdup(origin->env_name), 0);
		if (origin->has_value)
			tmp->env_value = gc_add(sh, ft_strdup(origin->env_value), 0);
		else
			tmp->env_value = NULL;
		tmp->exported = origin->exported;
		tmp->has_value = origin->has_value;
		tmp->env_printable = origin->env_printable;
		tmp->next = NULL;
		env_addcopy(&copy, tmp);
		origin = origin->next;
	}
	return (copy);
}

void	ft_swap_node(t_env	**first, t_env **second)
{
	t_env	tmp;

	tmp.env_name = (*first)->env_name;
	tmp.env_value = (*first)->env_value;
	tmp.exported = (*first)->exported;
	tmp.has_value = (*first)->has_value;
	tmp.env_printable = (*first)->env_printable;
	(*first)->env_name = (*second)->env_name;
	(*first)->env_value = (*second)->env_value;
	(*first)->exported = (*second)->exported;
	(*first)->has_value = (*second)->has_value;
	(*first)->env_printable = (*second)->env_printable;
	(*second)->env_name = tmp.env_name;
	(*second)->env_value = tmp.env_value;
	(*second)->exported = tmp.exported;
	(*second)->has_value = tmp.has_value;
	(*second)->env_printable = tmp.env_printable;
}

void	sort_env_list(t_env	**tmp)
{
	t_env	*first;
	t_env	*second;
	int		len;

	first = *tmp;
	while (first->next)
	{
		second = first->next;
		while (second)
		{
			if (ft_strlen(first->env_name) > ft_strlen(second->env_name))
				len = ft_strlen(first->env_name);
			else
				len = ft_strlen(second->env_name);
			if (ft_strncmp(first->env_name, second->env_name, len) > 0)
				ft_swap_node(&first, &second);
			second = second->next;
		}
		first = first->next;
	}
}

void	declare(t_sh *sh)
{
	t_env	*tmp;

	tmp = copy_env(sh);
	if (!tmp)
		return ;
	sort_env_list(&tmp);
	while (tmp)
	{
		if ((ft_strncmp(tmp->env_name, "PATH", 5) == 0 && sh->default_path)
			|| ft_strncmp(tmp->env_name, "_", 2) == 0)
			tmp = tmp->next;
		else if (tmp->exported)
		{
			if (tmp->has_value)
				ft_printf("declare -x %s=\"%s\"\n",
					tmp->env_name, tmp->env_value);
			else
				ft_printf("declare -x %s\n", tmp->env_name);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}
