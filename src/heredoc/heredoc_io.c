/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   heredoc_io.c                                         :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 14:16:20 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 14:17:22 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

void	read_heredoc_input(t_sh *sh, t_heredoc *data, int fd)
{
	char	*input;

	while (1)
	{
		input = gc_add(sh, readline(">"), 0);
		if (!input)
		{
			handle_heredoc_eof(sh, data->delimeter);
			exit(1);
		}
		if (g_sig == SIGINT)
			exit(g_sig + 128);
		if ((ft_strlen(input) == ft_strlen(data->delimeter))
			&& !ft_strncmp(input, data->delimeter, ft_strlen(input)))
			break ;
		input = expand_heredoc_input(sh, data->expand, input);
		if (!input)
			exit(1);
		input = gc_add(sh, ft_strjoin(input, "\n"), 0);
		write(fd, input, ft_strlen(input));
	}
	close(fd);
	exit(0);
}

char	*expand_heredoc_input(t_sh *sh, int q_type, char *input)
{
	if (q_type == S_QUOTE)
	{
		input = gc_add(sh, ft_strjoin("\'", input), 0);
		if (!input)
			return (allocate_error(sh), NULL);
		input = gc_add(sh, ft_strjoin(input, "\'"), 0);
		if (!input)
			return (allocate_error(sh), NULL);
	}
	else
	{
		input = gc_add(sh, ft_strjoin("\"", input), 0);
		if (!input)
			return (allocate_error(sh), NULL);
		input = gc_add(sh, ft_strjoin(input, "\""), 0);
		if (!input)
			return (allocate_error(sh), NULL);
	}
	return (expand_token(sh, input, &q_type));
}

int	handle_heredoc_eof(t_sh *sh, char *delimeter)
{
	char	*msg;
	char	*cmd_count;

	cmd_count = gc_add(sh, ft_itoa(sh->cmd_cnt), 0);
	msg = gc_add(sh,
			ft_strdup("minishell: Warning: here-document at line "), 0);
	msg = gc_add(sh, ft_strjoin(msg, cmd_count), 0);
	msg = gc_add(sh,
			ft_strjoin(msg, " delimeted by end-of-file (wanted '"), 0);
	msg = gc_add(sh, ft_strjoin(msg, delimeter), 0);
	msg = gc_add(sh, ft_strjoin(msg, "')"), 0);
	ft_putendl_fd(msg, 2);
	exit (-1);
}

int	heredoc_child(t_sh *sh, t_heredoc *data, int fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		read_heredoc_input(sh, data, fd);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sh->exit_status = WTERMSIG(status) + 128;
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return (-1);
	return (0);
}
