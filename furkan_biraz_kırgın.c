/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   furkan_kırgın.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:00:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/15 16:38:22 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static int	first_exec_arg(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i] && is_assignment(args[i]))
		i++;
	return (i);
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
			ft_putendl_fd(": No such file or directory", 2);
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

static void	child_process(t_sh *sh, t_command *cmd, char *path, char **argv)
{
	char	**envp;

	if (cmd->redirs && apply_redirections(sh, cmd->redirs) < 0)
		exit(1);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	envp = env_to_envp(sh);
	execve(path, argv, envp);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(argv[0], 2);
	ft_putendl_fd(": execution failed", 2);
	exit(126);
}

static void	exec_external(t_sh *sh, t_command *cmd, char **argv, int *stdinout)
{
	pid_t	pid;
	int		status;
	char	*path;

	dup2(stdinout[0], STDIN_FILENO);
	dup2(stdinout[1], STDOUT_FILENO);
	close(stdinout[0]);
	close(stdinout[1]);
	path = resolve_path(sh, cmd->args[0]);
	pid = fork();
	if (pid == 0)
		child_process(sh, cmd, path, argv);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
}

void	execute_cmd(t_sh *sh, t_command *cmd)
{
	int	saved_stdin_out[2];
	int	cmd_index;
	t_command	exec_cmd;

	if (!cmd)
		return ;
	saved_stdin_out[0] = dup(STDIN_FILENO);
	saved_stdin_out[1] = dup(STDOUT_FILENO);
	if (cmd->redirs && apply_redirections(sh, cmd->redirs) < 0)
	{
		dup2(saved_stdin_out[0], STDIN_FILENO);
		dup2(saved_stdin_out[1], STDOUT_FILENO);
		close(saved_stdin_out[0]);
		close(saved_stdin_out[1]);
		sh->exit_status = 1;
		return ;
	}
	if (handle_assignments(sh, cmd) == -1)
	{
		dup2(saved_stdin_out[0], STDIN_FILENO);
		dup2(saved_stdin_out[1], STDOUT_FILENO);
		close(saved_stdin_out[0]);
		close(saved_stdin_out[1]);
		sh->exit_status = 1;
		return ;
	}
	cmd_index = first_exec_arg(cmd->args);
	if (!cmd->args || !cmd->args[cmd_index])
	{
		dup2(saved_stdin_out[0], STDIN_FILENO);
		dup2(saved_stdin_out[1], STDOUT_FILENO);
		close(saved_stdin_out[0]);
		close(saved_stdin_out[1]);
		return ;
	}
	exec_cmd = *cmd;
	exec_cmd.args = &cmd->args[cmd_index];
	exec_cmd.builtin = identify_builtin(exec_cmd.args[0]);
	if (exec_cmd.builtin != NOT_BUILTIN)
	{
		exec_builtin(sh, &exec_cmd);
		dup2(saved_stdin_out[0], STDIN_FILENO);
		dup2(saved_stdin_out[1], STDOUT_FILENO);
		close(saved_stdin_out[0]);
		close(saved_stdin_out[1]);
		return ;
	}
	exec_external(sh, &exec_cmd, exec_cmd.args, saved_stdin_out);
}
