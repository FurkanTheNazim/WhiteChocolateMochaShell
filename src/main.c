/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/06/03 19:03:50 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	init_minishell(t_sh *sh)
{
	ft_bzero(sh, sizeof(t_sh));
	sh->fds[0] = -1;
	sh->fds[1] = -1;
}

void	reset_loop(t_sh *sh, t_gc *cp_cmd)
{
	normalize_env(sh);
	gc_rollback(sh, cp_cmd);
	sh->token_list = NULL;
	sh->input = NULL;
	sh->in_pipe = 0;
}

int	execute_lexer_and_expander(t_sh *sh, t_gc *cp_cmd)
{
	sh->token_list = lexer(sh);
	if (!sh->token_list || sh->syntax_error)
	{
		sh->syntax_error = 0;
		gc_rollback(sh, cp_cmd);
		sh->token_list = NULL;
		sh->input = NULL;
		return (-1);
	}
	if (validate_tokens(sh))
		return (reset_loop(sh, cp_cmd), -1);
	if (heredoc(sh) < 0)
		return (reset_loop(sh, cp_cmd), -1);
	if (expand_token_list(sh) < 0)
		return (reset_loop(sh, cp_cmd), -1);
	return (0);
}

void	arg_checker(int ac)
{
	if (ac != 1)
	{
		ft_putendl_fd("Usage: ./minishell", 2);
		exit (1);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_sh	sh;
	t_gc	*cp_cmd;

	arg_checker(ac);
	init_minishell(&sh);
	init_env(&sh, av[0], envp);
	term_pacifier();
	while (1)
	{
		cp_cmd = gc_checkpoint(&sh);
		get_input(&sh);
		if (execute_lexer_and_expander(&sh, cp_cmd) < 0)
			continue ;
		sh.ast = build_ast(&sh);
		if (sh.ast)
		{
			execute_ast(&sh, sh.ast);
			g_sig = 0;
		}
		reset_loop(&sh, cp_cmd);
	}
	return (0);
}
