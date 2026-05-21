/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   heredoc_v2.c                                         :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/14 17:23:35 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/21 17:42:17 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

void	find_cmd(t_token *list)
{
	t_token *tmp;
	t_token	*pre;

	pre = NULL;
	tmp = list;
	while (tmp && tmp->type != TOKEN_PIPE)
	{
		if ((tmp && tmp->type != TOKEN_HEREDOC) && (!pre || pre->type != TOKEN_HEREDOC))
		{
			tmp->is_heredoc = 1;
			break ;
		}
		pre = tmp;
		tmp = tmp->next;
	}
}

char	*cr_file_name(t_sh *sh, int cmd_index)
{
	char	*num;
	char	*file_name;

	num = gc_add(sh, ft_itoa(cmd_index), 0);
	if (!num)
		return (NULL);
	file_name = gc_add(sh, ft_strjoin("/tmp/heredoc_", num), 0);
	if (!file_name)
		return (NULL);
	return (file_name);
}

int	cr_heredoc_file(t_sh *sh, t_token *cmd)
{
	static unsigned int	file_index = 0;
	char				*file_name;
	t_token				*tmp;
	int					fd;

	tmp = cmd;
	file_name = cr_file_name(sh, file_index);
	if (!file_name)
		return (allocate_error(sh), -1);
	file_index++;
	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (ft_putendl_fd(strerror(errno), 2), -1);
	while (tmp)
	{
		if (tmp->is_heredoc == 1)
		{
			tmp->heredoc_file = gc_add(sh, ft_strdup(file_name), 0);
			break ;	
		}
		tmp = tmp->next;
	}
	return (fd);
}

int	init_data(t_sh *sh, t_token *list, t_heredoc *data)
{
	int	i;

	data->delimeter = list->next->value;
	i = ft_strlen(data->delimeter) -1;
	if (data->delimeter[0] == '\'' && data->delimeter[i] == '\'')
	{
		data->delimeter = gc_add(sh, ft_strtrim(data->delimeter, "\'"), 0);
		if  (!(data->delimeter))
			return (allocate_error(sh),-1);
		data->expand = S_QUOTE;
	}
	else if (data->delimeter[0] =='\"' && data->delimeter[i] == '\"')
	{
		data->delimeter = gc_add(sh, ft_strtrim(data->delimeter, "\""), 0);
		if (!(data->delimeter))
			return (allocate_error(sh), -1);
		data->expand = S_QUOTE;
	}
	else
		data->expand = D_QUOTE;
	return (0);
}

int	handle_heredoc_eof(t_sh *sh, char *delimeter)
{
	char	*msg;
	char	*cmd_count;

	cmd_count = gc_add(sh, ft_itoa(sh->cmd_cnt), 0);
	msg = gc_add(sh,ft_strdup("minishell: Warning: here-document at line "), 0);	
	msg = gc_add(sh, ft_strjoin(msg, cmd_count), 0);
	msg = gc_add(sh, ft_strjoin(msg, " delimeted by end-of-file (wanted '"), 0);
	msg = gc_add(sh, ft_strjoin(msg, delimeter), 0);
	msg = gc_add(sh, ft_strjoin(msg, "')"), 0);
	ft_putendl_fd(msg, 2);
	exit (-1);
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

void	read_heredoc_input(t_sh *sh, t_heredoc *data, int fd)
{
	char	*input;

	while(1)
	{
		input = gc_add(sh, readline(">"), 0);
		if (!input)
		{
			handle_heredoc_eof(sh, data->delimeter);
			exit(1);
		}
		if (g_sig == SIGINT)
			exit(g_sig + 128);
		if ((ft_strlen(input) == ft_strlen(data->delimeter)) &&
				!ft_strncmp(input, data->delimeter, ft_strlen(input)))
			break ;
		input = expand_heredoc_input(sh, data->expand, input);
		if (!input)
			exit(1);
		input = gc_add(sh, ft_strjoin(input, "\n"), 0);
		write(fd, input, ft_strlen(input));
	}
	exit(0);
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

int	heredoc(t_sh *sh)
{
	t_token		*list;
	t_token		*tmp;
	t_heredoc	data;
	int			fd;

	list = sh->token_list;
	tmp = list;
	setup_signal(2);
	while (list)
	{
		if (list->type == TOKEN_HEREDOC)
		{
			find_cmd(tmp);
			fd = cr_heredoc_file(sh, tmp);
			if (fd < 0)
				return (-1);
			if (init_data(sh, list, &data) < 0)
				return (-1);
			if (heredoc_child(sh, &data, fd) < 0)
				return (-1);
		}
		if (list->type == TOKEN_PIPE)
			tmp = list->next;
		list = list->next;
	}
	setup_signal(3);
	return (0);
}
