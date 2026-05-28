/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   expander.c                                           :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/12 20:12:04 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:15:49 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

t_segment	parse_next_segment(t_sh *sh, char *str, int *i, int *flag)
{
	t_segment	tmp;

	if (str[*i] == '\'')
		tmp = parse_single_quote(sh, str, i);
	else if (str[*i] == '\"')
		tmp = parse_double_quote(sh, str, i);
	else
		tmp = parse_unquote(sh, str, i);
	if (tmp.type == SEG_DOUBLE_QUOTE || tmp.type == SEG_SINGLE_QUOTE)
		*flag = 1;
	return (tmp);
}

void	init_expander_data(t_sh *sh, t_segment *seg, char **new_value)
{
	seg->expanded = NULL;
	*new_value = gc_add(sh, ft_strdup(""), 0);
}

char	*expand_token(t_sh *sh, char *value, int *flag)
{
	t_segment	seg;
	char		*new_value;
	int			i;

	i = 0;
	init_expander_data(sh, &seg, &new_value);
	while (value[i])
	{
		seg = parse_next_segment(sh, value, &i, flag);
		if (seg.fail || !new_value)
			return (NULL);
		if (seg.type != SEG_SINGLE_QUOTE && ft_strchr(seg.value, '$'))
			seg.expanded = expand_str(sh, seg);
		else
			seg.expanded = seg.value;
		if (!seg.expanded)
			continue ;
		new_value = gc_add(sh, gc_join(new_value, seg.expanded), 0);
	}
	if (!new_value[0] && !seg.expanded && !(*flag))
	{
		*flag = 1;
		return (NULL);
	}
	return (new_value);
}

int	expand_token_list(t_sh *sh)
{
	t_token	*tmp;
	t_gc	*cp;
	int		flag;

	flag = 0;
	cp = gc_checkpoint(sh);
	tmp = sh->token_list;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
		{
			tmp->value = expand_token(sh, tmp->value, &flag);
			if (!(tmp->value) && !flag)
			{
				gc_rollback(sh, cp);
				ft_putendl_fd("minishell: cannot allocate memory ", 2);
				sh->exit_status = 1;
				return (-1);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}
