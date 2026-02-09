#include "../../includes/WCMS.h"

void	*gc_malloc(t_minishell *shell, size_t n)
{
	void	*ptr;
	t_gc	*node;

	ptr = malloc(n);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->adrr = ptr;
	node->next = shell->gc;
	shell->gc = node;
	return (ptr);
}

void	gc_free(t_minishell *shell, void *ptr)
{
	t_gc	*cur;
	t_gc	*prev;

	if (!ptr || !shell->gc)
		return ;
	cur = shell->gc;
	prev = NULL;
	while (cur)
	{
		if (cur->adrr == ptr)
		{
			if (prev)
				prev->next = cur->next;
			else
				shell->gc = cur->next;
			free(cur->adrr);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void	gc_free_all(t_minishell *shell)
{
	t_gc	*tmp;

	while (shell->gc)
	{
		tmp = shell->gc;
		shell->gc = shell->gc->next;
		free(tmp->adrr);
		free(tmp);
	}
}

t_gc	*gc_checkpoint(t_minishell *shell)
{
	return (shell->gc);
}

void	gc_rollback(t_minishell *shell, t_gc *checkpoint)
{
	t_gc	*tmp;

	while (shell->gc && shell->gc != checkpoint)
	{
		tmp = shell->gc;
		shell->gc = shell->gc->next;
		free(tmp->adrr);
		free(tmp);
	}
}


void	gc_add(t_minishell *shell, void *ptr)
{
	t_gc	*node;

	if (!ptr)
		return ;
	node = malloc(sizeof(t_gc));
	if (!node)
	{
		free(ptr);
		return ;
	}
	node->adrr = ptr;
	node->next = shell->gc;
	shell->gc = node;
}