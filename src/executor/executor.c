/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:25:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/10 15:25:00 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static void	exec_pipe_left(t_sh *sh, t_ast_node *node, int *fd)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	execute_ast(sh, node->left);
	exit(sh->exit_status);
}

static void	exec_pipe_right(t_sh *sh, t_ast_node *node, int *fd)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	execute_ast(sh, node->right);
	exit(sh->exit_status);
}

static void	execute_pipe(t_sh *sh, t_ast_node *node)
{
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(fd) < 0)
		return ;
	pid_left = fork();
	if (pid_left == 0)
		exec_pipe_left(sh, node, fd);
	pid_right = fork();
	if (pid_right == 0)
		exec_pipe_right(sh, node, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
}

void	execute_ast(t_sh *sh, t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_CMD)
		execute_cmd(sh, node->cmd);
	else if (node->type == AST_PIPE)
		execute_pipe(sh, node);
}
