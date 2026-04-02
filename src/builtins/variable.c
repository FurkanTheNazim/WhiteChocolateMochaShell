/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 16:46:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/02 16:58:24 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

/*
** is_assignment: "FOO=bar" formatini kontrol eder
** Gecerli: isim alpha/_ ile baslamali, alnum/_ devam etmeli, '=' olmali
** "FOO=bar" -> 1, "echo" -> 0, "=bar" -> 0, "123=x" -> 0
*/
int	is_assignment(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] != '=')
		return (0);
	return (1);
}
