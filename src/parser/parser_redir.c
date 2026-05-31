/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:00:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/02/17 21:22:50 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

t_redir	*create_redir(t_sh *shell, int type, char *file)
{
	t_redir	*new_redir;

	new_redir = gc_malloc(shell, sizeof(t_redir), 0);
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->file = file;
	new_redir->heredoc_fd = -1;
	new_redir->next = NULL;
	return (new_redir);
}

void	add_redir(t_sh *shell, t_command *cmd, t_redir *new)
{
	t_redir	*tmp;

	(void)shell;
	if (!cmd->redirs)
	{
		cmd->redirs = new;
		return ;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	free_redir_list(t_sh *shell, t_redir *list)
{
	(void)shell;
	(void)list;
}
