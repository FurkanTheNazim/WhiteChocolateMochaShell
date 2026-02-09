#ifndef GC_H
# define GC_H

#include "WCMS.h"

typedef struct s_gc
{
	void		*adrr;
	struct s_gc	*next;	
}t_gc;

void	*gc_malloc(t_minishell *shell, size_t n);
void	gc_free(t_minishell *shell, void *ptr);
void	gc_free_all(t_minishell *shell);
t_gc	*gc_checkpoint(t_minishell *shell);
void	gc_rollback(t_minishell *shell, t_gc *checkpoint);
void	gc_add(t_minishell *shell, void *ptr);

#endif
