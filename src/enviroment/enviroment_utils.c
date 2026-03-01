#include "WCMS.h"

void	env_error(t_sh *sh, char *err)
{
	ft_putendl_fd(err, 2);
	gc_free_all(sh);
	exit(1);
}

t_env	*env_newnode(t_sh *sh, char *name, char *value)
{
	t_env	*node;

	if (!name)
		return (NULL);
	node = gc_malloc(sh,sizeof(t_env), 1);
	if (!node)
		return (NULL);
	node->env_name = name;
	node->env_value = value;
	node->exported = 1;
	node->has_value = 1;
	node->next = NULL;
	return (node);
}

int	env_addback(t_sh *sh, t_env *newnode)
{
	t_env	*tmp;

	if (!newnode)
		return (-1);
	if (!sh->env)
	{
		sh->env = newnode;
		return (1);
	}
	tmp = sh->env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = newnode;
	return (1);
}
