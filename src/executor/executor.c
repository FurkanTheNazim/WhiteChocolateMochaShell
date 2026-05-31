/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:25:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/05/05 01:43:57 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static void	exec_pipe_child(t_sh *sh, t_ast_node *node, int *fd, int is_left)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (is_left)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		sh->in_pipe = 1;
		execute_ast(sh, node->left);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		sh->in_pipe = 1;
		execute_ast(sh, node->right);
	}
	exit(sh->exit_status);
}

static void	wait_for_children(t_sh *sh, pid_t pid_left, pid_t pid_right)
{
	int	status;

	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->exit_status = WTERMSIG(status) + 128;
}

static void	execute_pipe(t_sh *sh, t_ast_node *node)
{
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(fd) < 0)
		return ;
	pid_left = fork();
	if (pid_left == 0)
		exec_pipe_child(sh, node, fd, 1);
	pid_right = fork();
	if (pid_right == 0)
		exec_pipe_child(sh, node, fd, 0);
	close(fd[0]);
	close(fd[1]);
	wait_for_children(sh, pid_left, pid_right);
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
