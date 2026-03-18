#include "../../includes/WCMS.h"

int	count_dollar_sign(char *str)
{
	int	i;

	i = 0;
	while (*str)
	{
		if (*str == '$')
			i++;
		str++;
	}
	return (i);
}

void	split_by_dollar(t_sh *sh, char **tmp, char *str, int n)
{
	int	i;

	i = 0;
	if (!str[i])
	{
		tmp[n] = NULL;
		return ;
	}
	if (str[i] == '$')
		i++;
	while (str[i] && str[i] != '$')
		i++;
	tmp[n] = gc_add(sh, ft_substr(str, 0, i), 0);
	split_by_dollar(sh,  tmp, &str[i], n + 1);
}

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

	while (str[*i] && ((ft_isalnum(str[*i]) || str[*i] == '_') || str[*i] =='?'))
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
	remain = gc_add(sh, ft_substr(tmp[n], i+1, ft_strlen(&tmp[n][i])), 0);
	if (!remain)
		return (-1);
	tmp[n] = gc_add(sh, ft_strjoin(name, remain), 0);
	if (!tmp[n])
		return (-1);
	if (expand_dollar_segment(sh, tmp, n + 1) < 0)
		return (-1);
	return (0);
}

int	joined_strlen(char **tmp, int n)
{
	int	i;

	i = 0;
	if (!tmp[n])
		return (0);
	while (tmp[n][i])
		i++;
	i += joined_strlen(tmp, n + 1);
	return (i);
}

char	*create_expanded(t_sh *sh, char **tmp)
{
	char	*expanded;
	int		n;
	int		i;
	int		j;

	i = 0;
	n = 0;
	expanded = gc_malloc(sh, sizeof(char) * joined_strlen(tmp, 0) + 1, 0);
	if (!expanded)
		return (NULL);
	while (tmp[n])
	{
		j = 0;
		while (tmp[n][j])
		{
			expanded[i] = tmp[n][j];
			i++;
			j++;
		}
		n++;
	}
	expanded[i] = '\0';
	return (expanded);
}
static char	*word_splitter(t_sh *sh, char *str)
{
	char	**tmp;
	char	*result;
	int		i;

	i = 0;
	result = "";
	tmp = gc_add(sh, ft_split(str, ' '), 0);
	if (!tmp)
		return (allocate_error(sh), NULL);
	while (tmp[i])
	{
		result = gc_add(sh, ft_strjoin(result, tmp[i]), 0);
		if (!result)
			return (allocate_error(sh), NULL);
		if (tmp[i +1])
			result = gc_add(sh, ft_strjoin(result, " "), 0);
		if (!result)
			return (allocate_error(sh), NULL);
		i++;
	}
	if (!tmp[0])
		return (NULL);
	return (result);
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

t_segment	parse_single_quote(t_sh *sh, char *str, int *i)
{
	t_segment	tmp;
	int			start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	tmp.value = ft_substr(str, start, *i -start);
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
	tmp.value = ft_substr(str, start, *i -start);
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

t_segment	parse_next_segment(t_sh *sh, char *str, int *i, int *flag)
{
	t_segment tmp;

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


char	*expand_token(t_sh *sh, char *value, int *flag)
{
	t_segment	seg;
	char		*new_value;
	int			i;

	i = 0;
	new_value = gc_add(sh, ft_strdup(""), 0);
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
	t_token *tmp;
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
			if(!(tmp->value) && !flag)
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
