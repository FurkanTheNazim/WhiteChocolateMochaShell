#include "WCMS.h"

void	export_err(t_sh *sh, char	*param)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(param, 2);
	ft_putendl_fd("': not a valid identfier", 2);
	sh->exit_status = 1;
}

int validate_param(t_sh *sh, char *str)
{
    int i;

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


int	param_count(char **param)
{
	int	i;

	i = 0;
	while (param && param[i])
		i++;
	return (i);
}

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
		if (ft_strncmp(tmp->env_name, "PATH", 4) == 0 && sh->default_path)
			tmp = tmp->next;
		else if (tmp->exported)
		{
			if (tmp->has_value)
				ft_printf("declare -x %s=\"%s\"\n", tmp->env_name, tmp->env_value);
			else
				ft_printf("declare -x %s\n", tmp->env_name);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
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
		sh->exit_status	= 1;
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
		ret[1] = gc_add(sh, ft_substr(str, i + 1, ft_strlen(str +i +1)), 1);
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
			tmp->exported =1;
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
					env_addback(sh, env_newnode(sh, name_value[0], name_value[1]));
			}
			i++;
		}
	}
}
