#include "../../includes/WCMS.h"

void	fonc(t_sh *sh)
{
	gc_malloc(sh, sizeof(char) * 10, 0);
	gc_add(sh, ft_split("merhaba dunya" , ' '), 0);
}

int main()
{
	t_sh sh;
	ft_bzero(&sh, sizeof(t_sh));
	fonc(&sh);
	gc_free_all(&sh);
}