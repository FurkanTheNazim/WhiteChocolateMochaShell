/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   lexer_token.c                                        :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/01/09 18:00:00 by minishell           #+#    #+#           */
/*   Updated: 2026/05/08 05:58:55 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

t_token	*newnode(t_sh *sh, char *raw, char *token)
{
	t_token	*node;

	if (!gc_add(sh, token, 0))
		return (NULL);
	node = gc_malloc(sh, sizeof(t_token), 0);
	if (!node)
		return (NULL);
	node->raw = raw;
	node->value = token;
	node->type = TOKEN_WORD;
	node->quote_type = 0;
	node->next = NULL;
	return (node);
}

int	addback(t_sh *sh, t_token *newnode)
{
	t_token	*temp;

	if (!sh || !newnode)
		return (allocate_error(sh), -1);
	if (!sh->token_list)
	{
		sh->token_list = newnode;
		return (1);
	}
	temp = sh->token_list;
	while (temp->next)
		temp = temp->next;
	temp->next = newnode;
	return (1);
}

void	free_node(t_sh *sh, t_token **list)
{
	t_token	*temp;

	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
			gc_free(sh, (*list)->value, 1);
		gc_free(sh, *list, 1);
		*list = temp;
	}
	*list = NULL;
}
