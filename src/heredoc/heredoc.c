/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   heredoc.c                                            :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/14 17:23:35 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/28 14:17:29 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"
#include <errno.h>

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
			if (process_heredoc(sh, list, &data, fd) < 0)
				return (-1);
			close(fd);
		}
		if (list->type == TOKEN_PIPE)
			tmp = list->next;
		list = list->next;
	}
	return (setup_signal(3), 0);
}
