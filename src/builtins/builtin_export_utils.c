/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   builtin_export_utils.c                               :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 19:31:04 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 19:31:57 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

int	param_count(char **param)
{
	int	i;

	i = 0;
	while (param && param[i])
		i++;
	return (i);
}

void	export_err(t_sh *sh, char	*param)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(param, 2);
	ft_putendl_fd("': not a valid identfier", 2);
	sh->exit_status = 1;
}
