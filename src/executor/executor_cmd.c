/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:00:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/15 17:40:14 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

static void	close_fds(t_sh *sh)
{
	dup2(sh->fds[0], STDIN_FILENO);
	dup2(sh->fds[1], STDOUT_FILENO);
	close(sh->fds[0]);
	close(sh->fds[1]);
}

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
	else if (redir->type == TOKEN_HEREDOC)
		fd = redir->heredoc_fd;
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

static void	child_process(t_sh *sh, t_command *cmd, char *path)
{
	char	**envp;

	if (cmd->redirs && apply_redirections(sh, cmd->redirs) < 0)
		exit(1);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	envp = env_to_envp(sh);
	execve(path, cmd->args, envp);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putendl_fd(": execution failed", 2);
	exit(126);
}

static void	exec_external(t_sh *sh, t_command *cmd)
{
	pid_t	pid;
	int		status;
	char	*path;

	close_fds(sh);
	path = resolve_path(sh, cmd->args[0]);
	update_lastcmd_env(sh, cmd, path, 0);
	pid = fork();
	if (pid == 0)
		child_process(sh, cmd, path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
}


void	execute_cmd(t_sh *sh, t_command *cmd)
{ 
	if (!cmd)
		return ;
	handle_assignments(sh, cmd);
	if (!cmd->args || !(cmd->args[0]))
		return ;
	sh->fds[0] = dup(STDIN_FILENO);
	sh->fds[1] = dup(STDOUT_FILENO);
	if (cmd->redirs && apply_redirections(sh, cmd->redirs) < 0)
	{
		sh->exit_status = 1;
		close_fds(sh);
		return ;
	}
	if (cmd->builtin != NOT_BUILTIN)
	{
		exec_builtin(sh, cmd);
		close_fds(sh);
		return ;
	}
	exec_external(sh, cmd);
}
	