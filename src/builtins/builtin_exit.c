#include "../../includes/WCMS.h"

void	exit_err(char *err, char *param)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (param)
	{
		ft_putstr_fd(param, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(err, 2);
	return ;
}

long	ft_atol(char *param, int *flag)
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
		exit_err("numeric argument required", param);
		*flag = 1;
		return (-1);
	}
	return ((long)(result));
}

int	validate_parameter(t_sh *sh, char *param)
{
	int	i;

	i  = 0;
	while (param[i])
	{
		if (!ft_isdigit(param[i]))
		{
			exit_err("numeric argument required", param);
			sh->exit_status = 2;
			return (0);
		}
		i++;
	}
	return (1);
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
		exit_err("too many arguments", NULL);
		sh->exit_status = 2;
		return ;
	}
	value = ft_atol(param[1], &flag);
	if (flag)
		return ;
	sh->exit_status = value % 256;
	exit(sh->exit_status);
}

// int main()
// {
// 	t_sh sh;
// 	int i = 0;
// 	ft_bzero(&sh, sizeof(t_sh));
// 	char **param;
// 	param = malloc(sizeof(char *)* 4);
// 	param[0] = ft_strdup("exit");
// 	param[1] = ft_strdup("122a");
// 	param[2] = ft_strdup("123");
// 	param[3] = NULL;

// 	builtin_exit(&sh, param);
// 	printf("%d", sh.exit_status);
// 	free(param[0]);
// 	free(param[1]);
// 	free(param[2]);
// 	free(param[3]);
// 	free(param);
// }
