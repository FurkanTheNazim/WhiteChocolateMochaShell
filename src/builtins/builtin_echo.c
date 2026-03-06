#include "WCMS.h"

int	validate_flag(char *flag)
{
	int	i;

	i = 1;
	if (flag[0] != '-' && flag[1] != 'n')
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
	
	flag = 0;
	i = 1;
	while (param[i])
	{
		if(validate_flag(param[i]) && validate_flag(param[1]))
			flag = 1;
		else
		{
			ft_printf("%s", param[i]);
			if (param[i +1])
				ft_printf(" ");
		}
		i++;
	}
	if (!flag)
	{
		printf("\n");
		sh->exit_status = 0;
	}
}
