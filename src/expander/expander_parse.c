/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   expander_parse.c                                     :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 19:01:51 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:16:28 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

t_segment	parse_single_quote(t_sh *sh, char *str, int *i)
{
	t_segment	tmp;
	int			start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	tmp.value = ft_substr(str, start, *i - start);
	if (!tmp.value)
	{
		tmp.fail = 1;
		return (tmp);
	}
	gc_add(sh, tmp.value, 0);
	tmp.type = SEG_SINGLE_QUOTE;
	if (str[*i] == '\'')
		(*i)++;
	tmp.fail = 0;
	return (tmp);
}

t_segment	parse_double_quote(t_sh *sh, char *str, int *i)
{
	t_segment	tmp;
	int			start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\"')
		(*i)++;
	tmp.value = ft_substr(str, start, *i - start);
	if (!tmp.value)
	{
		tmp.fail = 1;
		return (tmp);
	}
	gc_add(sh, tmp.value, 0);
	tmp.type = SEG_DOUBLE_QUOTE;
	if (str[*i] == '\"')
		(*i)++;
	tmp.fail = 0;
	return (tmp);
}

t_segment	parse_unquote(t_sh *sh, char *str, int *i)
{
	t_segment	tmp;
	int			start;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '\"')
		(*i)++;
	tmp.value = ft_substr(str, start, *i - start);
	if (!tmp.value)
	{
		tmp.fail = 1;
		return (tmp);
	}
	gc_add(sh, tmp.value, 0);
	tmp.type = SEG_NO_QUOTE;
	tmp.fail = 0;
	return (tmp);
}
