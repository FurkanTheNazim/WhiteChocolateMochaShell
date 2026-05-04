/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:11:00 by kedemiro          #+#    #+#             */
/*   Updated: 2026/05/04 20:36:31 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static char *echo_join(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}

int	validate_flag(char *flag)
{
	int	i;

	i = 1;
	if (flag[0] != '-' || flag[1] != 'n')
		return (0);
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (0);	
		i++;
	}
	return (1);
}

static char *combine_words(char *tmp, char *param, int *write_start)
{
	if (*write_start)
		tmp = echo_join(tmp, " ");
	*write_start = 1;
	tmp = echo_join(tmp, param);
	return (tmp);
}

void	builtin_echo(t_sh *sh, char **param)
{
	int		i;
	int		flag;
	int		write_start;
	char	*tmp;

	write_start = 0;
	flag = 0;
	i = 1;
	tmp = ft_strdup("");
	while (param[i])
	{
		if(!write_start && (validate_flag(param[i]) && validate_flag(param[1])))
			flag = 1;
		else
			tmp = combine_words(tmp, param[i], &write_start);
		i++;
	}
	if (!flag)
		tmp = echo_join(tmp, "\n");
	ft_putstr_fd(tmp, 1);
	free(tmp);
	sh->exit_status = 0;
}
