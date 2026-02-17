#include "../includes/WCMS.h"

void	env_splitter(t_sh *sh, char *str, char *ptr)
{
	int	i;
	char	*first;
	char	*second;

	i = 0;
	while (str[i])
	{
		// printf("str[i]%p, ptr: %p\n", str+i, ptr);
		if (str + i == ptr)
		{
			first = ft_substr(str, 0, i);
			if (first)
			{
				gc_add(sh, first);
				sh->env->env_name = first;//seg
			}
			else
				exit(-1/* hata kodunu kontrol et sonra */);
			second = ft_substr(str, i + 1, ft_strlen(ptr + 1));
			if (second)
			{
				gc_add(sh,second);
				sh->env->env_value = second;
			}
			else
				exit(-1/* hata kodunu kontrol et sonra*/);
		}
		i++;
	}
}

void	init_env(t_sh *sh, char **env)
{
	char	*ptr;
	int		i;

	i = 0;
	sh->env = gc_malloc(sh, sizeof(t_env), 1);
	while (env[i])
	{
		ptr = ft_strchr(env[i], '=');
		if (ptr)
			env_splitter(sh, env[i], ptr);
		printf("%s=%s\n", sh->env->env_name, sh->env->env_value);
		i++;
	}
	printf("%s=%s\n", sh->env->env_name, sh->env->env_value);
}

int	main(int ac, char **av, char **env)
{
	t_sh	sh;
	ft_bzero(&sh, sizeof(t_sh));
	init_env(&sh, env);
}
