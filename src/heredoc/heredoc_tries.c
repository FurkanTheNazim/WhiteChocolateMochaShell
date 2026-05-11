/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   heredoc_tries.c                                      :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/07 03:34:10 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/11 17:08:45 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

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

int	handle_heredoc_eof(t_sh *sh, t_heredoc *data)
{
	char	*msg;
	char	*cmd_count;

	cmd_count = gc_add(sh, ft_itoa(sh->cmd_cnt), 0);
	msg = gc_add(sh,ft_strdup("minishell: Warning: here-document at line "), 0);	
	msg = gc_add(sh, ft_strjoin(msg, cmd_count), 0);
	msg = gc_add(sh, ft_strjoin(msg, " delimeted by end-of-file (wanted '"), 0);
	msg = gc_add(sh, ft_strjoin(msg, data->delimeter), 0);
	msg = gc_add(sh, ft_strjoin(msg, "')"), 0);
	ft_putendl_fd(msg, 2);
	return (-1);
}

int	cr_tmp_file(t_sh *sh, t_token *tmp, t_heredoc *data)
{
	static int	cmd_index = 0;
	int			fd;
	char		*input;

	data->start->heredoc_file = cr_file_name(sh, cmd_index);
	fd = open(data->start->heredoc_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (ft_putendl_fd(strerror(errno), 2), -1);
	tmp->heredoc_file = data->start->heredoc_file;
	while (1)
	{
		setup_signal_heredoc();
		input = gc_add(sh, readline(">"), 0);
		if (!input)
			return (handle_heredoc_eof(sh, data));
		input = expand_heredoc_input(sh, data->expand, input);	
		if (!input)
			return (allocate_error(sh), -1);
		if ((ft_strlen(input) == ft_strlen(data->delimeter)) &&
				!ft_strncmp(input, data->delimeter, ft_strlen(input)))
			break ;
		input = gc_add(sh, ft_strjoin(input, "\n"), 0);
		if (g_sig == SIGINT)
		{
			sh->exit_status = g_sig + 128;
			g_sig = 0;
			return (-1);
		}
		write(fd, input, ft_strlen(input));
	}
	data->start->is_heredoc = 1;
	cmd_index += 1;
	close(fd);
	setup_signal(2);
	return (0);
}

int	check_expand(t_sh *sh, t_heredoc *data)
{
	int	i;

	i = ft_strlen(data->delimeter);
	if (data->delimeter[0] == '\'' && data->delimeter[i] == '\'')
	{
		data->delimeter = gc_add(sh, ft_strtrim(data->delimeter, "\'"), 0);
		if  (!(data->delimeter))
			return (-1);
		data->expand = S_QUOTE;
	}
	else if (data->delimeter[0] =='\"' && data->delimeter[i] == '\"')
	{
		data->delimeter = gc_add(sh, ft_strtrim(data->delimeter, "\""), 0);
		if (!(data->delimeter))
			return (-1);
		data->expand = S_QUOTE;
	}
	else
		data->expand = D_QUOTE;
	return (0);
}

void	cut_token_list(t_token *start, t_token *end)
{
	while (start)
	{
		if (start->next == end)
		{
			start->next = end->next->next;
			break ;
		}
		start = start->next;
	}
}

int	is_heredoc(t_sh *sh)
{
	t_token		*tmp;
	t_heredoc	data;//norm için sh->heredoc.XXX yapılabilir.
	ft_bzero(&data, sizeof(data));
	tmp = sh->token_list;
	while (tmp)
	{
		data.start = tmp;
		while (tmp && tmp->type != TOKEN_PIPE)
		{
			if (tmp->type == TOKEN_HEREDOC)
			{
				if (!(tmp->next))
					return (-1);//Geçici bu. Önceside lexer - heredoc arası syntax kontrol olmalı 
				data.delimeter = tmp->next->value;
				if (check_expand(sh, &data) < 0)
					return (allocate_error(sh) ,-1);
				cut_token_list(data.start, tmp);
			}
			if (data.delimeter)
			{
				if (cr_tmp_file(sh, data.start, &data) < 0)
					return (-1);
				data.delimeter = NULL;
			}
			tmp = tmp->next;
		}
		if (tmp && tmp->type == TOKEN_PIPE)
			tmp = tmp->next;
	}
	return (0);
}
