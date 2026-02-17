/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:05:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/02/17 21:22:52 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

int	is_redir_token(int type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || \
			type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

//Pipe'a kadar olan tokenları atlar, pipe yoksa listenin sonunu bulana kadar döner
 

t_token	*skip_to_pipe(t_token *token)
{
	while (token && token->type != TOKEN_PIPE)
		token = token->next;
	if (token && token->type == TOKEN_PIPE)
		return (token->next);
	return (NULL);
}

/*
** Bir komut bolumundeki argumanlari sayar.
** Pipe veya liste sonunda durur.
** Yonlendirme (redirection) tokenlarini ve dosya isimlerini atlar.
*/
int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (is_redir_token(token->type))
		{
			if (token->next)
				token = token->next;
		}
		else
		{
			count++;
		}
		token = token->next;
	}
	return (count);
}
