#include "WCMS.h"

int	validate_env(t_sh *sh)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = sh->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->env_name, "PWD", 3) == 0)
			i += 1;
		else if (!ft_strncmp(tmp->env_name, "SHLVL", 5) == 0)
			i += 1;
		else if (!ft_strncmp(tmp->env_name, "_", 1) == 0)
			i += 1;
		tmp = tmp->next;
	}
	if (i == 3)
		return  (1);
	else
		return (0);
}

void	manuel_init(t_sh *sh, char *av)
{
	char	*pwd;

	pwd = gc_malloc(sh, 4048, 1);
	if (!pwd)
		exit (1);
	if (getcwd(pwd, 4048) == NULL)
		env_error(sh, "minishell: pwd: getcwd\n");
	env_addback(sh, env_newnode(sh, "PWD", pwd));
	env_addback(sh, env_newnode(sh, "SHLVL", "1"));
	env_addback(sh, env_newnode(sh, "_", av));
	if (!validate_env(sh))
		env_error(sh, "minishell: enviroment error\n");
}

char	**env_delimeter(t_sh *sh, char *envp, int i)
{
	char	**ret;

	ret = gc_malloc(sh, (sizeof(char *) * 3), 1);
	if (!ret)
		env_error(sh, "minishell: enviroment error\n");
	while (envp[i])
	{
		if (envp[i] == '=')
			break ;
		i++;
	}
	if (envp[i] == '\0')
	{
		ret[0] = gc_add(sh, ft_strdup(envp), 1);
		ret[1] = gc_add(sh, ft_strdup(""), 1);
	}
	else
	{
		ret[0] = gc_add(sh, ft_substr(envp, 0, i), 1);
		ret[1] = gc_add(sh, ft_substr(envp, i +1, ft_strlen(envp +i +1)), 1);
	}
	ret[2] = NULL;
	return (ret);
}

void	add_to_list(t_sh *sh, char *envp)
{
	char	**data;
	int		i;
	int		shlvl;

	i = 0;
	data = env_delimeter(sh,  envp, i);
	if (!data[0] || !data[1])
		env_error(sh, "minishell: enviroment error\n");
	if (ft_strncmp(data[0], "SHLVL", 5) == 0)
	{
		shlvl = ft_atoi(data[1]);
		gc_free(sh, data[1], 1);
		if (shlvl >= 999)
		{
			ft_putstr_fd("minishell: warning: shell level (", 2);
			ft_putnbr_fd(shlvl, 2);
			ft_putendl_fd(") too high, resetting to 1", 2);
			data[1] = gc_add(sh, ft_itoa(1), 1);
		}
		else
			data[1] = gc_add(sh, ft_itoa(shlvl+1), 1);
	}
	if (!env_addback(sh, env_newnode(sh, data[0], data[1])))
		env_error(sh, "minishell: enviroment error\n");
}

void	init_env(t_sh *sh, char *av ,char **envp)
{
	int		i;
	int		flag;

	flag = 0;
	i = 0;
	if (!envp)
		env_error(sh, "minishell: enviroment error\n");
	if (!envp[0])
		manuel_init(sh, av);
	else
	{
		while (envp[i])
		{
			if (ft_strncmp(envp[i], "PATH=", 5) == 0)
				flag = 1;
			add_to_list(sh, envp[i]);
			i++;
		}
	}
	if (!flag)
	{
		if (!env_addback(sh, env_newnode(sh, "PATH", DEFAULT_PATH)))
			env_error(sh, "minishell: enviroment error\n");
		sh->default_path = 1;
	}
}
