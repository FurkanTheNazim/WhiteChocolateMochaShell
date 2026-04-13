/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 20:11:06 by kedemiro          #+#    #+#             */
/*   Updated: 2026/04/13 14:36:02 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

long	ft_atol(t_sh *sh, char *param, int *flag)
{
	long long int	sign;
	long long int	result;

	result = 0;
	sign = 1;
	while ((*param >= 9 && *param <= 13) || *param == 32)
		param++;
	if (*param == '-' || *param == '+')
	{
		if (*param == '-')
			sign *= -1;
		param++;
	}
	while (*param >= '0' && *param <= '9')
	{
		result = (result * 10) + (*param - 48);
		param++;
	}
	if (result * sign > LLONG_MAX || result * sign < LLONG_MIN)
	{
		exit_err(sh, "numeric argument required", param, 2);
		*flag = 1;
		return (-1);
	}
	return (((long)(result)) * sign);
}

static void	check_overflow(t_sh *sh, char *param, int *data)
{
	int		i;
	char	*lim;

	if (data[1] < 19)
		return ;
	lim = "9223372036854775807";
	if (data[2] == -1)
		lim = "9223372036854775808";
	i = data[0];
	while (data[1] == 19 && param[i])
	{
		if (param[i] != lim[i - data[0]])
		{
			if (param[i] > lim[i - data[0]])
				data[2] = 2;
			break ;
		}
		i++;
	}
	if (data[2] == 2 || data[1] > 19)
	{
		exit_err(sh, "numeric argument required", param, 2);
		exit(2);
	}
}

int	validate_parameter(t_sh *sh, char *param)
{
	int	i;
	int	data[3];

	i = 0;
	data[1] = 0;
	data[2] = 0;
	while ((param[i] >= 9 && param[i] <= 13) || param[i] == 32)
		i++;
	if (param[i] == '-' || param[i] == '+')
		data[2] = 44 - param[i++];
	while (param[i] == '0')
		i++;
	data[0] = i;
	while (param[i])
	{
		if (!ft_isdigit(param[i]))
		{
			exit_err(sh, "numeric argument required", param, 2);
			exit (2);
		}
		data[1]++;
		i++;
	}
	return (check_overflow(sh, param, data), 1);
}

void	builtin_exit(t_sh *sh, char **param)
{
	long	value;
	int		flag;

	flag = 0;
	if (!(sh->in_pipe))
		ft_putendl_fd("exit", 2);
	if (!param[1])
	{
		gc_free_all(sh);
		exit(0);
	}
	if (!validate_parameter(sh, param[1]))
		return ;
	if (param[2])
	{
		exit_err(sh, "too many arguments", NULL, 1);
		return ;
	}
	value = ft_atol(sh, param[1], &flag);
	if (flag)
		return ;
	sh->exit_status = value % 256;
	exit(sh->exit_status);
}
