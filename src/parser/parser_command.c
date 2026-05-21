/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   parser_command.c                                     :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/02/09 19:55:00 by mahmmous            #+#    #+#           */
/*   Updated: 2026/05/15 01:46:24 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

// GC (Garbage Collector) kullanarak yeni boş bir komut düğümü oluşturur
t_command	*create_command(t_sh *shell)
{
	t_command	*new_cmd;

	new_cmd = gc_malloc(shell, sizeof(t_command), 0);
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->redirs = NULL;
	new_cmd->builtin = NOT_BUILTIN;
	new_cmd->heredoc_file = NULL;
	new_cmd->is_heredoc = 0;
	new_cmd->next = NULL;
	return (new_cmd);
}

// Komutun argüman dizisine GC kullanarak bir argüman ekler
void	add_arg(t_sh *shell, t_command *cmd, t_token *token)
{
	char	**new_args;
	int		len;
	int		i;

	len = 0;
	if (cmd->args)
	{
		while (cmd->args[len])
			len++;
	}
	new_args = gc_malloc(shell, sizeof(char *) * (len + 2), 0);
	if (!new_args)
		return ;
	i = 0;
	while (i < len)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = token->value;
	new_args[i + 1] = NULL;
	cmd->heredoc_file = token->heredoc_file;
	cmd->is_heredoc = token->is_heredoc;
	cmd->args = new_args;
}

// Komut listesinin sonuna bir komut ekler
void	add_command(t_sh *shell, t_command **list, t_command *new)
{
	t_command	*tmp;

	(void)shell;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

// Bellek GC geri alma (rollback) ile yönetilir, bu yüzden bu işlevsiz olabilir veya gerekirse özelleştirilebilir
void	free_command_list(t_sh *shell, t_command *list)
{
	(void)shell;
	(void)list;
}
