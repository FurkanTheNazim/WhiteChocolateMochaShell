// /* ************************************************************************** */
// /*                                                                            */
// /*                                                          :::      :::::::: */
// /*   main.c                                               :+:      :+:    :+: */
// /*                                                      +:+ +:+         +:+   */
// /*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
// /*                                                  +#+#+#+#+#+   +#+         */
// /*   Created: 2026/01/09 18:00:00 by minishell           #+#    #+#           */
// /*   Updated: 2026/05/15 01:39:25 by kedemiro           ###   ########.fr     */
// /*                                                                            */
// /* ************************************************************************** */

// #include "WCMS.h"

// void	print_expanded(t_token *list)
// {
// 	while (list)
// 	{
// 		if (list->value)
// 			printf("%s",list->value);
// 		list = list->next;
// 	}
// 	printf("\n");
// }

// void	init_minishell(t_sh *sh)
// {
// 	ft_bzero(sh, sizeof(t_sh));
// 	sh->fds[0] = -1;
// 	sh->fds[1] = -1;
// 	// sh->token_list = NULL;
// 	// sh->input = NULL;
// 	// sh->envp = NULL;
// 	// sh->exit_status = 0;
// 	// sh->syntax_error = 0;
// }

// static int	check_ischar(char	*input)
// {
// 	int	i;

// 	i = 0;
// 	if (input[i] == '\n')
// 		return (0);
// 	while (input[i] && ft_isspace(input[i]))
// 		i++;
// 	if (input[i])
// 		return (1);
// 	return (0);
// }

// // static void	print_tokens(t_token *list)
// // {
// // 	int	i;

// // 	printf("token printer\n");
// // 	i = 0;
// // 	while (list)
// // 	{
// // 		printf("i: %d || type[%d] : %s\n", i, list->type, list->value);
// // 		printf("quote type [%d]\n", list->quote_type);
// // 		list = list->next;
// // 		i++;
// // 	}
// // }

// // static const char	*builtin_name(int type)
// // {
// // 	const char	*names[] = {"NOT_BUILTIN", "echo", "cd", "pwd",
// // 		"export", "unset", "env", "exit"};

// // 	if (type >= 0 && type <= 7)
// // 		return (names[type]);
// // 	return ("unknown");
// // }

// // static void	print_commands(t_command *cmd)
// // {
// // 	int	i;
// // 	int	j;
// // 	t_redir	*r;

// // 	printf("command printer\n");
// // 	i = 0;
// // 	while (cmd)
// // 	{
// // 		printf("--- Command %d ---\n", i++);
// // 		printf("  Builtin: %s (%d)\n", builtin_name(cmd->builtin),
// // 			cmd->builtin);
// // 		if (cmd->args)
// // 		{
// // 			printf("  Args: ");
// // 			j = 0;
// // 			while (cmd->args[j])
// // 			{
// // 				printf("[%s] ", cmd->args[j]);
// // 				j++;
// // 			}
// // 			printf("\n");
// // 		}
// // 		if (cmd->redirs)
// // 		{
// // 			printf("  Redirs: ");
// // 			r = cmd->redirs;
// // 			while (r)
// // 			{
// // 				printf("(Type:%d File:%s) ", r->type, r->file);
// // 				r = r->next;
// // 			}
// // 			printf("\n");
// // 		}
// // 		cmd = cmd->next;
// // 	}
// // 	printf("------------------\n");
// // }

// // static void	cleanup_loop(t_minishell *sh)
// // {
// // 	if (sh->token_list)
// // 	{
// // 		free_node(&sh->token_list);
// // 		sh->token_list = NULL;
// // 	}
// // 	if (sh->input)
// // 	{
// // 		free(sh->input);
// // 		sh->input = NULL;
// // 	}
// // }

// static int	handle_eof(t_sh *sh)
// {
// 	rl_clear_history();
// 	gc_free_all(sh);
// 	if (isatty(STDIN_FILENO))
// 		ft_putendl_fd("exit", 2);
// 	if (sh->fds[0] != -1)
// 		close(sh->fds[0]);
// 	if (sh->fds[1] != -1)
// 		close(sh->fds[1]);
// 	exit (sh->exit_status);
// }

// int	newline_handler(t_sh *sh)
// {
// 	static char	**split = NULL;
// 	static int	stop = 0;

// 	if (!split && !ft_strchr(sh->input, '\n'))
// 		return (0);
// 	if (!split)
// 	{
// 		split = ft_split(sh->input, '\n');
// 		if (!split)
// 			return (allocate_error(sh), -1);
// 		sh->newline = 1;
// 	}
// 	if ((stop -1) >= 0)
// 		free(split[stop - 1]);
// 	if (!split[stop])
// 	{
// 		free(split);
// 		sh->newline = 0;
// 		split = NULL;
// 		return (-1);
// 	}
// 	sh->input = split[stop++];
// 	return (0);
// }

// void	get_input(t_sh *sh)
// {
// 	while (1)
// 	{
// 		setup_signal(1);
// 		if (sh->newline == 0)
// 		{
// 			if (isatty(STDIN_FILENO))
// 				sh->input = readline("mochashell>");
// 			else
// 			{
// 				sh->input = get_next_line(STDIN_FILENO);
// 				if (sh->input && ft_strlen(sh->input) > 0 && sh->input[ft_strlen(sh->input) - 1] == '\n')
// 					sh->input[ft_strlen(sh->input) - 1] = '\0';
// 			}
// 			if (!sh->input)	
// 				handle_eof(sh);
// 			gc_add(sh, sh->input, 0);
// 			if (g_sig == SIGINT)
// 			{
// 				sh->exit_status = g_sig + 128;
// 				g_sig = 0;
// 			}
// 		}
// 		if ((newline_handler(sh) < 0))
// 			continue ;
// 		if (!check_ischar(sh->input))
// 			continue ;
// 		break ;
// 	}
// 	add_history(sh->input);	
// 	sh->cmd_cnt += 1;
// 	setup_signal(2);
// 	return ;
// }

// int	execute_lexer_and_expander(t_sh *sh, t_gc *cp_cmd)
// {
// 	sh->token_list = lexer(sh);
// 	if (!sh->token_list || sh->syntax_error)
// 	{
// 		sh->syntax_error = 0;
// 		gc_rollback(sh, cp_cmd);
// 		sh->token_list = NULL;
// 		sh->input = NULL;
// 		return (-1);
// 	}
// 	if (validate_tokens(sh))
// 	{
// 		gc_rollback(sh, cp_cmd);
// 		sh->token_list = NULL;
// 		sh->input = NULL;
// 		return (-1);
// 	}
// 	if (heredoc(sh) < 0)
// 	{
// 		gc_rollback(sh, cp_cmd);
// 		sh->token_list = NULL;
// 		sh->input = NULL;
// 		return (-1);
// 	}
// 	if  (expand_token_list(sh) < 0)
// 	{
// 		gc_rollback(sh, cp_cmd);
// 		sh->token_list = NULL;
// 		sh->input = NULL;
// 		return (-1);
// 	}
// 	return (0);
// }

// int	main(int ac, char **av, char **envp)
// {
// 	t_sh	sh;
// 	t_gc	*cp_cmd;

// 	if (ac != 1)
// 	{
// 		ft_putendl_fd("Usage: ./minishell", 2);
// 		exit (1);
// 	}
// 	init_minishell(&sh);
// 	init_env(&sh, av[0], envp);
// 	term_pacifier();
// 	while (1)
// 	{
// 		cp_cmd = gc_checkpoint(&sh);
// 		get_input(&sh);
// 		if (execute_lexer_and_expander(&sh, cp_cmd) < 0)
// 			continue ;
// 		sh.ast = build_ast(&sh);
// 		if (sh.ast)
// 		{
// 			execute_ast(&sh, sh.ast);
// 			g_sig = 0;
// 		}
// 		normalize_env(&sh);
// 		gc_rollback(&sh, cp_cmd);
// 		sh.token_list = NULL;
// 		sh.input = NULL;
// 		sh.in_pipe = 0;
// 	}
// 	return (0);
// }

// // int	main(int ac, char **av, char **envp)
// // {
// // 	t_sh	sh;
// // 	t_gc	*cp_cmd;

// // 	if (ac != 1)
// // 	{
// // 		ft_putendl_fd("Usage: ./minishell", 2);
// // 		exit (1);
// // 	}
// // 	init_minishell(&sh);
// // 	init_env(&sh, av[0], envp);
// // 	while (1)
// // 	{
// // 		cp_cmd = gc_checkpoint(&sh);
// // 		if (sh.newline == 0)
// // 		{
// // 			sh.input = readline("mochashell>");
// // 			gc_add(&sh, sh.input, 0);
// // 			if (!sh.input)
// // 				return (handle_eof(&sh));
// // 		}
// // 		if (newline_handler(&sh) < 0)
// // 			continue ;
// // 		add_history(sh.input);
// // 		sh.token_list = lexer(&sh);
// // 		if (!sh.token_list || sh.syntax_error)
// // 		{
// // 			sh.syntax_error = 0;
// // 			gc_rollback(&sh, cp_cmd);
// // 			sh.token_list = NULL;
// // 			sh.input = NULL;
// // 			continue ;
// // 		}
// // 		// if (handle_heredoc(&sh) == -1)
// // 		// {
// // 		// 	gc_rollback(&sh, cp_hrd);
// // 		// 	continue ;
// // 		// }
// // 		if (expand_token_list(&sh) < 0)
// // 		{
// // 			continue;
// // 		}
// // 		// ft_printf("[----------]");
// // 		// // builtin_env(&sh,NULL);
// // 		// builtin_pwd(&sh, NULL);
// // 		// char *param[3];
// // 		// param[0] = "cd";
// // 		// param[1] = sh.token_list->value;
// // 		// param[2] = NULL;
// // 		// builtin_cd(&sh, param);
// // 		// builtin_pwd(&sh, NULL);
// // 		// // param[3] = "medsrhaba";
// // 		// param[1] = NULL;
// // 		// // builtin_echo(&sh, param);
// // 		// builtin_export(&sh, param);
// // 		// ft_printf("{----\n}");
// // 		// builtin_env(&sh, NULL);
// // 		// print_tokens(sh.token_list);
// // 		sh.ast = build_ast(&sh);
// // 		if (sh.ast)
// // 			execute_ast(&sh, sh.ast);
// // 		normalize_env(&sh);
// // 		gc_rollback(&sh, cp_cmd);
// // 		sh.token_list = NULL;
// // 		sh.input = NULL;
// // 	}
// // 	return (0);
// // }


// // int main(int ac, char **av, char **envp)
// // {
// //     t_sh    sh;
// //     t_gc    *cp_cmd;
// //     t_env   *dbg;

// //     if (ac != 1)
// //     {
// //         ft_putendl_fd("Usage: ./minishell", 2);
// //         exit(1);
// //     }
// //     init_minishell(&sh);
// //     init_env(&sh, av[0], envp);
// //     while (1)
// //     {
// //         cp_cmd = gc_checkpoint(&sh);
// //         sh.input = readline("mochashell>");
// //         gc_add(&sh, sh.input, 0);
// //         if (!sh.input)
// //             return (handle_eof(&sh));
// //         add_history(sh.input);
// //         sh.token_list = lexer(&sh);
// //         if (sh.syntax_error)
// //         {
// //             sh.syntax_error = 0;
// //             gc_rollback(&sh, cp_cmd);
// //             sh.token_list = NULL;
// //             sh.input = NULL;
// //             continue ;
// //         }
// //         expand_token_list(&sh);
// //         builtin_env(&sh, NULL);
// //         char *param[3];
// //         param[0] = "export";
// //         param[1] = "TEST=123";
// //         param[2] = NULL;
// //         builtin_export(&sh, param);
// //         printf("=== EXPORT SONRASI ===\n");
// //         dbg = sh.env;
// //         while (dbg)
// //         {
// //             printf("ENV: %s=%s\n", dbg->env_name, dbg->env_value ? dbg->env_value : "(null)");
// //             dbg = dbg->next;
// //         }
// //         printf("=== ROLLBACK SONRASI ===\n");
// //         dbg = sh.env;
// //         while (dbg)
// //         {
// // 			printf("ENV: %s=%s\n", dbg->env_name, dbg->env_value ? dbg->env_value : "(null)");
// //             dbg = dbg->next;
// //         }
// //         printf("\n\n\n\n");
// //         builtin_env(&sh, NULL);
// //         print_tokens(sh.token_list);
// //         sh.command_list = parser(&sh);
// //         print_commands(sh.command_list);
// // 		gc_rollback(&sh, cp_cmd);
// //         sh.token_list = NULL;
// //         sh.input = NULL;
// //     }
// //     return (0);
// // }
