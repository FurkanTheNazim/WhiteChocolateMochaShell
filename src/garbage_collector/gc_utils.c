#include "../../includes/WCMS.h"

// flag: 1 ise işaretli (kalıcı), 0 ise işaretsiz (geçici)
void	*gc_malloc(t_sh *shell, size_t n, int flag)
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
	node->addr = ptr;
	node->mark = flag;
	node->next = shell->gc;
	shell->gc = node;
	return (ptr);
}

// Manuel free: Sadece mark edilmemişse (0) siler
void	gc_free(t_sh *shell, void *ptr)
{
	t_gc	*cur;
	t_gc	*prev;

	if (!ptr || !shell->gc)
		return ;
	cur = shell->gc;
	prev = NULL;
	while (cur)
	{
		// Sadece adres eşleşirse VE mark 0 (işaretsiz) ise sil
		if (cur->addr == ptr)
		{
			if (cur->mark == 1) // İşaretliyse dokunma
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

// Rollback: Checkpoint'e kadar olan listede SADECE işaretsizleri siler, işaretlileri korur
void	gc_rollback(t_sh *shell, t_gc *checkpoint)
{
	t_gc	*cur;
	t_gc	*prev;
	t_gc	*tmp;

	cur = shell->gc;
	prev = NULL;
	// Liste başından checkpoint noktasına kadar tara
	while (cur && cur != checkpoint)
	{
		if (cur->mark == 0) // İşaretsiz ise sil
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
		else // İşaretli ise atla ve listede tut
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

// Her şeyi temizler (Program kapanırken veya kritik reset anında çağrılır)
// Flag değerine bakmaksızın her şeyi siler.
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

t_gc	*gc_checkpoint(t_sh *shell)
{
	return (shell->gc);
}

void	gc_add(t_sh *shell, void *ptr, int flag)
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
	node->addr = ptr;
	node->mark = flag;
	node->next = shell->gc;
	shell->gc = node;
}