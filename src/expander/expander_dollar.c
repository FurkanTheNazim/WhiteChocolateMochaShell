/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   expander_dollar.c                                    :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 19:03:17 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:17:10 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

char	*get_env_value(t_sh *sh, char *str)
{
	t_env	*tmp;
	size_t	len;
	char	*result;

	if (!str || !str[0])
		return (NULL);
	len = ft_strlen(str);
	tmp = sh->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->env_name, str, len + 1))
		{
			if (!tmp->env_value)
				return (NULL);
			result = gc_add(sh, ft_strdup(tmp->env_value), 0);
			if (!result)
				return (NULL);
			return (result);
		}
		tmp = tmp->next;
	}
	return ("");
}

char	*resolve_var_name(t_sh *sh, char *str, int *i)
{
	char	*tmp;

	while (str[*i] && ((ft_isalnum(str[*i]) || str[*i] == '_')
			|| str[*i] == '?'))
		(*i)++;
	tmp = gc_add(sh, ft_substr(str, 0, *i), 0);
	if (!tmp)
		return (NULL);
	if (ft_strncmp(tmp, "?", 1) == 0)
		tmp = gc_add(sh, ft_itoa(sh->exit_status), 0);
	else
		tmp = get_env_value(sh, tmp);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	expand_dollar_segment(t_sh *sh, char **tmp, int n)
{
	int		i;
	char	*name;
	char	*remain;

	if (tmp[n] == NULL)
		return (0);
	if (tmp[n][0] != '$' || (tmp[n][0] == '$' && !tmp[n][1]))
		return (expand_dollar_segment(sh, tmp, n + 1));
	i = 0;
	name = resolve_var_name(sh, &tmp[n][1], &i);
	if (!name)
		return (-1);
	remain = gc_add(sh, ft_substr(tmp[n], i + 1, ft_strlen(&tmp[n][i])), 0);
	if (!remain)
		return (-1);
	tmp[n] = gc_add(sh, ft_strjoin(name, remain), 0);
	if (!tmp[n])
		return (-1);
	if (expand_dollar_segment(sh, tmp, n + 1) < 0)
		return (-1);
	return (0);
}

char	*expand_str(t_sh *sh, t_segment seg)
{
	char	**tmp;
	char	*result;
	int		sign_c;

	sign_c = count_dollar_sign(seg.value);
	if (seg.value[0] != '$')
		sign_c++;
	tmp = gc_malloc(sh, (sizeof(char *) * (sign_c + 1)), 0);
	if (!tmp)
		return (NULL);
	split_by_dollar(sh, tmp, &seg.value[0], 0);
	if (expand_dollar_segment(sh, tmp, 0) < 0)
		return (NULL);
	result = create_expanded(sh, tmp);
	if (!result)
		return (NULL);
	if (seg.type == SEG_NO_QUOTE)
		result = word_splitter(sh, result);
	return (result);
}
