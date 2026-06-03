/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/06/03 18:55:30 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	init_minishell(t_sh *sh)
{
	ft_bzero(sh, sizeof(t_sh));
	sh->fds[0] = -1;
	sh->fds[1] = -1;
}

static int	check_ischar(char	*input)
{
	int	i;

	i = 0;
	if (input[i] == '\n')
		return (0);
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i])
		return (1);
	return (0);
}

static int	handle_eof(t_sh *sh)
{
	rl_clear_history();
	gc_free_all(sh);
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	if (sh->fds[0] != -1)
		close(sh->fds[0]);
	if (sh->fds[1] != -1)
		close(sh->fds[1]);
	exit (sh->exit_status);
}

void	reset_loop(t_sh *sh, t_gc *cp_cmd)
{
	normalize_env(sh);
	gc_rollback(sh, cp_cmd);
	sh->token_list = NULL;
	sh->input = NULL;
	sh->in_pipe = 0;
}

int	newline_handler(t_sh *sh)
{
	static char	**split = NULL;
	static int	stop = 0;

	if (!split && !ft_strchr(sh->input, '\n'))
		return (0);
	if (!split)
	{
		split = ft_split(sh->input, '\n');
		if (!split)
			return (allocate_error(sh), -1);
		sh->newline = 1;
	}
	if ((stop -1) >= 0)
		free(split[stop - 1]);
	if (!split[stop])
	{
		free(split);
		sh->newline = 0;
		stop = 0;
		split = NULL;
		return (-1);
	}
	sh->input = split[stop++];
	return (0);
}

void	read_input(t_sh *sh)
{
	if (sh->newline == 0)
	{
		if (isatty(STDIN_FILENO))
			sh->input = readline("mochashell>");
		else
		{
			sh->input = get_next_line(STDIN_FILENO);
			if (sh->input && ft_strlen(sh->input) > 0
				&& sh->input[ft_strlen(sh->input) - 1] == '\n')
				sh->input[ft_strlen(sh->input) - 1] = '\0';
		}
		if (!sh->input)
			handle_eof(sh);
		gc_add(sh, sh->input, 0);
		if (g_sig == SIGINT)
		{
			sh->exit_status = g_sig + 128;
			g_sig = 0;
		}
	}
}

void	get_input(t_sh *sh)
{
	while (1)
	{
		setup_signal(1);
		read_input(sh);
		if ((newline_handler(sh) < 0))
			continue ;
		if (!check_ischar(sh->input))
			continue ;
		break ;
	}
	add_history(sh->input);
	sh->cmd_cnt += 1;
	return ;
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
