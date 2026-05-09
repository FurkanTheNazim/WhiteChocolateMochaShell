/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   heredoc_tries.c                                      :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/07 03:34:10 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/09 13:33:41 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

char	*cr_file_name(t_sh *sh, int *cmd_index)
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

int	create_tmp_dir(t_sh *sh, t_heredoc *data)
{
	static int	cmd_index = 0;
	int			fd;
	char		*file_name;
	char		*input;

	fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (strerror(errno), -1);
	while (1)
	{
		input = readline(">");
		if (!input)
			return (-1);
	}
}

void	check_expand(t_token *node, t_heredoc *data)
{
	if (!(node->next))
		return ;
}

int	is_heredoc(t_sh *sh)
{
	t_token		*tmp;
	t_heredoc	data;

	tmp = sh->token_list;
	while (tmp)
	{
		data.start = tmp;	
		while (tmp && tmp->type != TOKEN_PIPE)
		{
			if (tmp->type == TOKEN_HEREDOC)
			{
				data.flag = 1;
				check_expand(tmp, &data);
			}
			data.end = tmp;
			tmp = tmp->next;
		}	
		if (tmp->type == TOKEN_PIPE && data.flag == 1)
		{
			if (create_tmp_dir(sh, &data) < 0)
				return (-1);
		}
		tmp = tmp->next;
	}
	return (0);
}
