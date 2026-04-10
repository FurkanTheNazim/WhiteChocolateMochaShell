#ifndef GC_H
# define GC_H

typedef struct s_sh t_sh;//circular include problemi böyle çözülürmüş

typedef struct s_gc
{
	int			mark;
	void		*addr;
	struct s_gc	*next;	
}t_gc;

void	*gc_malloc(t_sh *shell, size_t n, int flag);
void	gc_free(t_sh *shell, void *ptr, int key);
void	gc_free_all(t_sh *shell);
t_gc	*gc_checkpoint(t_sh *shell);
void	gc_rollback(t_sh *shell, t_gc *checkpoint);
void	*gc_add(t_sh *shell, void *ptr, int flag);
char	*gc_join(char *s1, char *s2);
void	*gc_add_matrix(t_sh *sh, void **ptr, int flag);

#endif
