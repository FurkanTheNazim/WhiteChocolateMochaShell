/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   gc_cleaner.c                                         :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 18:49:43 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 18:55:11 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	gc_free(t_sh *shell, void *ptr, int key)
{
	t_gc	*cur;
	t_gc	*prev;

	if (!ptr || !shell->gc)
		return ;
	cur = shell->gc;
	prev = NULL;
	while (cur)
	{
		if (cur->addr == ptr)
		{
			if (cur->mark == 1 && key == 0)
				return ;
			if (prev)
				prev->next = cur->next;
			else
				shell->gc = cur->next;
			free(cur->addr);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void	gc_rollback(t_sh *shell, t_gc *checkpoint)
{
	t_gc	*cur;
	t_gc	*prev;
	t_gc	*tmp;

	cur = shell->gc;
	prev = NULL;
	while (cur && cur != checkpoint)
	{
		if (cur->mark == 0)
		{
			tmp = cur;
			if (prev)
				prev->next = cur->next;
			else
				shell->gc = cur->next;
			cur = cur->next;
			free(tmp->addr);
			free(tmp);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

void	gc_free_all(t_sh *shell)
{
	t_gc	*tmp;

	while (shell->gc)
	{
		tmp = shell->gc;
		shell->gc = shell->gc->next;
		free(tmp->addr);
		free(tmp);
	}
}
