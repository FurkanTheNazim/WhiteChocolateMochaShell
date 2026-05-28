/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   expander_utils.c                                     :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 19:04:33 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:17:23 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

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
	split_by_dollar(sh, tmp, &str[i], n + 1);
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

char	*word_splitter(t_sh *sh, char *str)
{
	char	**tmp;
	char	*result;
	int		i;

	i = 0;
	result = "";
	tmp = gc_add_matrix(sh, (void **)ft_split(str, ' '), 0);
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
