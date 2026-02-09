#ifndef GC_H
# define GC_H

#include "WCMS.h"

typedef struct s_gc
{
	void		*adrr;
	struct s_gc	*next;	
}t_gc;

void	*gc_malloc(t_sh *shell, size_t n);
void	gc_free(t_sh *shell, void *ptr);
void	gc_free_all(t_sh *shell);
t_gc	*gc_checkpoint(t_sh *shell);
void	gc_rollback(t_sh *shell, t_gc *checkpoint);
void	gc_add(t_sh *shell, void *ptr);

#endif
