/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   heredoc_utils.c                                      :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/28 14:14:45 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 14:17:52 by kedemiro           ###   ########.fr     */
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

void	find_cmd(t_token *list)
{
	t_token	*tmp;
	t_token	*pre;

	pre = NULL;
	tmp = list;
	while (tmp && tmp->type != TOKEN_PIPE)
	{
		if ((tmp && tmp->type != TOKEN_HEREDOC)
			&& (!pre || pre->type != TOKEN_HEREDOC))
		{
			tmp->is_heredoc = 1;
			break ;
		}
		pre = tmp;
		tmp = tmp->next;
	}
}

int	init_data(t_sh *sh, t_token *list, t_heredoc *data)
{
	int	i;

	data->delimeter = list->next->value;
	i = ft_strlen(data->delimeter) - 1;
	if (data->delimeter[0] == '\'' && data->delimeter[i] == '\'')
	{
		data->delimeter = gc_add(sh, ft_strtrim(data->delimeter, "\'"), 0);
		if (!(data->delimeter))
			return (allocate_error(sh), -1);
		data->expand = S_QUOTE;
	}
	else if (data->delimeter[0] == '\"' && data->delimeter[i] == '\"')
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

int	process_heredoc(t_sh *sh, t_token *list, t_heredoc *data, int fd)
{
	if (fd < 0)
		return (-1);
	if (init_data(sh, list, data) < 0)
		return (-1);
	if (heredoc_child(sh, data, fd) < 0)
		return (-1);
	return (0);
}
