/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:11:00 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/15 18:08:45 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

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

void	builtin_echo(t_sh *sh, char **param)
{
	int	i;
	int	flag;
	int	write_start;

	write_start = 0;
	flag = 0;
	i = 1;
	while (param[i])
	{
		if(!write_start && (validate_flag(param[i]) && validate_flag(param[1])))
			flag = 1;
		else
		{
			write_start = 1; 
			ft_printf("%s", param[i]);
			if (param[i +1])
				ft_printf(" ");
		}
		i++;
	}
	if (!flag)
		printf("\n");
	sh->exit_status = 0;
}
