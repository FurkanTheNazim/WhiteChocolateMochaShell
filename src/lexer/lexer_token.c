/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/01/09 18:14:46 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

t_token	*create_newnode(char *raw, t_lexer_state state, char *token)
{
	t_token	*node;

	if (!token)
		return (NULL);
	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->raw = raw;
	node->value = token;
	node->type = TOKEN_WORD;
	if (state.in_double_quote)
		node->quote_type = D_QUOTE;
	else if (state.in_single_quote)
		node->quote_type = S_QUOTE;
	else
		node->quote_type = N_QUOTE;
	node->next = NULL;
	return (node);
}

void	addback(t_minishell *shell, t_token *newnode)
{
	t_token	*temp;

	if (!shell->token_list)
	{
		shell->token_list = newnode;
		return ;
	}
	temp = shell->token_list;
	while (temp->next)
		temp = temp->next;
	temp->next = newnode;
}

void	free_node(t_token **list)
{
	t_token	*temp;

	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
			free((*list)->value);
		free(*list);
		*list = temp;
	}
	*list = NULL;
}
