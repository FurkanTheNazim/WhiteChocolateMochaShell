/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

static int	open_redir(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	apply_redirections(t_sh *sh, t_redir *redirs)
{
	int	fd;
	int	target;

	(void)sh;
	while (redirs)
	{
		fd = open_redir(redirs);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redirs->file, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			return (-1);
		}
		if (redirs->type == TOKEN_REDIR_IN)
			target = STDIN_FILENO;
		else
			target = STDOUT_FILENO;
		dup2(fd, target);
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}

void	close_fds(t_sh *sh, t_command *cmd)
{
	if (cmd->builtin != BUILTIN_EXIT)
	{
		dup2(sh->fds[0], STDIN_FILENO);
		dup2(sh->fds[1], STDOUT_FILENO);
		close(sh->fds[0]);
		close(sh->fds[1]);
	}
}

void	setup_heredoc_fd(t_command *cmd)
{
	int	fd;

	if (!cmd->is_heredoc)
		return ;
	fd = open(cmd->heredoc_file, O_RDONLY);
	unlink(cmd->heredoc_file);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: cannot create temp file ", 2);
		ft_putstr_fd("for here-document: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}
