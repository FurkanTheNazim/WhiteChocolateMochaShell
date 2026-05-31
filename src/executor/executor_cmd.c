/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   executor_cmd.c                                       :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/04/10 15:00:00 by mahmmous            #+#    #+#           */
/*   Updated: 2026/05/28 13:37:17 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

void	child_process(t_sh *sh, t_command *cmd, char *path)
{
	char	**envp;
	char	*msg;
	char	*tmp;

	if (cmd->redirs && apply_redirections(sh, cmd->redirs) < 0)
		exit(1);
	if (!path)
		handle_cmd_not_found(cmd);
	envp = env_to_envp(sh);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, cmd->args, envp);
	msg = ft_strjoin("minishell: ", cmd->args[0]);
	tmp = ft_strjoin(msg, ": execution failed\n");
	ft_putstr_fd(tmp, 2);
	free(msg);
	free(tmp);
	exit(126);
}

static void	exec_in_main(t_sh *sh, t_command *cmd)
{
	pid_t	pid;

	if (cmd->builtin != NOT_BUILTIN)
	{
		sh->fds[0] = dup(STDIN_FILENO);
		sh->fds[1] = dup(STDOUT_FILENO);
		if (cmd->redirs && apply_redirections(sh, cmd->redirs) < 0)
		{
			sh->exit_status = 1;
			return (close_fds(sh, cmd));
		}
		exec_builtin(sh, cmd);
		close_fds(sh, cmd);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			setup_heredoc_fd(cmd);
			child_process(sh, cmd, resolve_path(sh, cmd->args[0]));
		}
		wait_single_child(sh, pid);
	}
}

static void	exec_in_pipe(t_sh *sh, t_command *cmd)
{
	if (cmd->builtin != NOT_BUILTIN)
	{
		if (cmd->redirs && (apply_redirections(sh, cmd->redirs) < 0))
			exit(1);
		exec_builtin(sh, cmd);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		exit(sh->exit_status);
	}
	else
	{
		setup_heredoc_fd(cmd);
		child_process(sh, cmd, resolve_path(sh, cmd->args[0]));
	}
}

void	execute_cmd(t_sh *sh, t_command *cmd)
{
	if (!cmd)
		return ;
	handle_assignments(sh, cmd);
	if (!cmd->args || !(cmd->args[0]))
		return ;
	if (sh->in_pipe == 0)
		exec_in_main(sh, cmd);
	else
		exec_in_pipe(sh, cmd);
}
