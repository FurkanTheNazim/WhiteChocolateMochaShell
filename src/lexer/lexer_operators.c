/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/01/09 18:01:29 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static int	get_pipe_type(char *ptr, int *i)
{
	(*i)++;
	if (ptr[*i] == '|')
	{
		(*i)++;
		return (TOKEN_OR);
	}
	return (TOKEN_PIPE);
}

static int	get_redir_in_type(char *ptr, int *i)
{
	(*i)++;
	if (ptr[*i] == '<')
	{
		(*i)++;
		return (TOKEN_HEREDOC);
	}
	return (TOKEN_REDIR_IN);
}

static int	get_redir_out_type(char *ptr, int *i)
{
	(*i)++;
	if (ptr[*i] == '>')
	{
		(*i)++;
		return (TOKEN_REDIR_APPEND);
	}
	return (TOKEN_REDIR_OUT);
}

int	append_operator(char *ptr, t_sh *shell)
{
	int		i;
	int		type;
	t_token	*node;
	char	*substr;

	i = 0;
	type = TOKEN_WORD;
	if (ptr[i] == '|')
		type = get_pipe_type(ptr, &i);
	else if (ptr[i] == '<')
		type = get_redir_in_type(ptr, &i);
	else if (ptr[i] == '>')
		type = get_redir_out_type(ptr, &i);
	substr = ft_substr(ptr, 0, i);
	if (substr)
		gc_add(shell, substr);
	node = create_newnode(shell, NULL, substr);
	if (node)
		node->type = type;
	addback(shell, node);
	return (i);
}
