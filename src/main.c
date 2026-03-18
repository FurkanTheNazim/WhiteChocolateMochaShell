/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/02/10 20:00:41 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	print_expanded(t_token *list)
{
	while (list)
	{
		if (list->value)
			printf("%s",list->value);
		list = list->next;
	}
	printf("\n");
}

void	init_minishell(t_sh *sh)
{
	ft_bzero(sh, sizeof(t_sh));
	// sh->token_list = NULL;
	// sh->input = NULL;
	// sh->envp = NULL;
	// sh->exit_status = 0;
	// sh->syntax_error = 0;
}

static void	print_tokens(t_token *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("i: %d || type[%d] : %s\n", i, list->type, list->value);
		printf("quote type [%d]\n", list->quote_type);
		list = list->next;
		i++;
	}
}

static void	print_commands(t_command *cmd)
{
	int	i;
	int	j;
	t_redir	*r;

	i = 0;
	while (cmd)
	{
		printf("--- Command %d ---\n", i++);
		if (cmd->args)
		{
			printf("  Args: ");
			j = 0;
			while (cmd->args[j])
			{
				printf("[%s] ", cmd->args[j]);
				j++;
			}
			printf("\n");
		}
		if (cmd->redirs)
		{
			printf("  Redirs: ");
			r = cmd->redirs;
			while (r)
			{
				printf("(Type:%d File:%s) ", r->type, r->file);
				r = r->next;
			}
			printf("\n");
		}
		cmd = cmd->next;
	}
	printf("------------------\n");
}

// static void	cleanup_loop(t_minishell *sh)
// {
// 	if (sh->token_list)
// 	{
// 		free_node(&sh->token_list);
// 		sh->token_list = NULL;
// 	}
// 	if (sh->input)
// 	{
// 		free(sh->input);
// 		sh->input = NULL;
// 	}
// }

static int	handle_eof(t_sh *sh)
{
	rl_clear_history();
	gc_free_all(sh);
	return (sh->exit_status);
}

int	main(int ac, char **av, char **envp)
{
	t_sh	sh;
	t_gc	*cp_cmd;

	if (ac != 1)
	{
		ft_putendl_fd("Usage: ./minishell", 2);
		exit (1);
	}
	init_minishell(&sh);
	init_env(&sh,av[0], envp);
	while (1)
	{
		cp_cmd = gc_checkpoint(&sh);
		sh.input = readline("mochashell>");
		gc_add(&sh, sh.input, 0);
		if (!sh.input)
			return (handle_eof(&sh));
		add_history(sh.input);
		sh.token_list = lexer(&sh);
		if (!sh.token_list || sh.syntax_error)
		{
			sh.syntax_error = 0;
			gc_rollback(&sh, cp_cmd);
			sh.token_list = NULL;
			sh.input = NULL;
			continue ;
		}
		// if (handle_heredoc(&sh) == -1)
		// {
		// 	gc_rollback(&sh, cp_hrd);
		// 	continue ;
		// }
		expand_token_list(&sh);
		// ft_printf("[----------]");
		// // builtin_env(&sh,NULL);
		builtin_pwd(&sh, NULL);
		char *param[3];
		param[0] = "cd";
		param[1] = sh.token_list->value;
		param[2] = NULL;
		builtin_cd(&sh, param);
		builtin_pwd(&sh, NULL);
		// // param[3] = "medsrhaba";
		// param[1] = NULL;
		// // builtin_echo(&sh, param);
		// builtin_export(&sh, param);
		// ft_printf("{----\n}");
		// builtin_env(&sh, NULL);
		print_expanded(sh.token_list);
		// executor(&sh);
		print_tokens(sh.token_list);
		sh.command_list = parser(&sh);
		print_commands(sh.command_list);
		gc_rollback(&sh, cp_cmd);
		sh.token_list = NULL;
		sh.input = NULL;
	}
	return (0);
}


// int main(int ac, char **av, char **envp)
// {
//     t_sh    sh;
//     t_gc    *cp_cmd;
//     t_env   *dbg;

//     if (ac != 1)
//     {
//         ft_putendl_fd("Usage: ./minishell", 2);
//         exit(1);
//     }
//     init_minishell(&sh);
//     init_env(&sh, av[0], envp);
//     while (1)
//     {
//         cp_cmd = gc_checkpoint(&sh);
//         sh.input = readline("mochashell>");
//         gc_add(&sh, sh.input, 0);
//         if (!sh.input)
//             return (handle_eof(&sh));
//         add_history(sh.input);
//         sh.token_list = lexer(&sh);
//         if (sh.syntax_error)
//         {
//             sh.syntax_error = 0;
//             gc_rollback(&sh, cp_cmd);
//             sh.token_list = NULL;
//             sh.input = NULL;
//             continue ;
//         }
//         expand_token_list(&sh);
//         builtin_env(&sh, NULL);
//         char *param[3];
//         param[0] = "export";
//         param[1] = "TEST=123";
//         param[2] = NULL;
//         builtin_export(&sh, param);
//         printf("=== EXPORT SONRASI ===\n");
//         dbg = sh.env;
//         while (dbg)
//         {
//             printf("ENV: %s=%s\n", dbg->env_name, dbg->env_value ? dbg->env_value : "(null)");
//             dbg = dbg->next;
//         }
//         printf("=== ROLLBACK SONRASI ===\n");
//         dbg = sh.env;
//         while (dbg)
//         {
// 			printf("ENV: %s=%s\n", dbg->env_name, dbg->env_value ? dbg->env_value : "(null)");
//             dbg = dbg->next;
//         }
//         printf("\n\n\n\n");
//         builtin_env(&sh, NULL);
//         print_tokens(sh.token_list);
//         sh.command_list = parser(&sh);
//         print_commands(sh.command_list);
// 		gc_rollback(&sh, cp_cmd);
//         sh.token_list = NULL;
//         sh.input = NULL;
//     }
//     return (0);
// }
