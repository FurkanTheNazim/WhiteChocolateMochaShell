/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/01/10 08:26:45 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	init_minishell(t_sh *shell)
{
	ft_bzero(shell, sizeof(t_sh));
	// shell->token_list = NULL;
	// shell->input = NULL;
	// shell->envp = NULL;
	// shell->exit_status = 0;
	// shell->syntax_error = 0;
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

// static void	cleanup_loop(t_minishell *shell)
// {
// 	if (shell->token_list)
// 	{
// 		free_node(&shell->token_list);
// 		shell->token_list = NULL;
// 	}
// 	if (shell->input)
// 	{
// 		free(shell->input);
// 		shell->input = NULL;
// 	}
// }

static int	handle_eof(t_sh *shell)
{
	rl_clear_history();
	gc_free_all(shell);
	return (shell->exit_status);
}

int	main(/*int ac, char **av, char **env*/)
{
	t_sh	shell;
	t_gc		*cp_cmd;

	init_minishell(&shell);
	// init_env(&shell, envp);
	while (1)
	{
		cp_cmd = gc_checkpoint(&shell);
		shell.input = readline("mochashell>");
		gc_add(&shell, shell.input);
		if (!shell.input)
			return (handle_eof(&shell));
		add_history(shell.input);
		shell.token_list = lexer(&shell);
		if (shell.syntax_error)
		{
			shell.syntax_error = 0;
			gc_rollback(&shell, cp_cmd);
			shell.token_list = NULL;
			shell.input = NULL;
			continue ;
		}
		// if (handle_heredoc(&shell) == -1)
		// {
		// 	gc_rollback(&shell, cp_hrd);
		// 	continue ;
		// }
		// expander(&shell);
		// executor(&shell);
		print_tokens(shell.token_list);
		gc_rollback(&shell, cp_cmd);
		shell.token_list = NULL;
		shell.input = NULL;
	}
	return (0);
}

