/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   gc_utils.c                                           :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/12 20:12:07 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 18:51:25 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

void	*gc_malloc(t_sh *shell, size_t n, int flag)
{
	void	*ptr;
	t_gc	*node;

	ptr = ft_calloc(1, n);
	if (!ptr)
		return (NULL);
	node = ft_calloc(1, sizeof(t_gc));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->addr = ptr;
	node->mark = flag;
	node->next = shell->gc;
	shell->gc = node;
	return (ptr);
}

t_gc	*gc_checkpoint(t_sh *shell)
{
	return (shell->gc);
}

void	*gc_add(t_sh *shell, void *ptr, int flag)
{
	t_gc	*node;

	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->addr = ptr;
	node->mark = flag;
	node->next = shell->gc;
	shell->gc = node;
	return (ptr);
}

char	*gc_join(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

void	*gc_add_matrix(t_sh *sh, void **ptr, int flag)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		gc_add(sh, ptr[i], flag);
		i++;
	}
	gc_add(sh, ptr, flag);
	return (ptr);
}
