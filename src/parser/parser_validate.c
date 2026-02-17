/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:20:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/02/17 21:23:00 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

int	is_redir_token(int type); // parser_utils.c dosyasından

static int	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

static int	check_pipe_syntax(t_token *t)
{
	// İlk token'ın Pipe olup olmadığını kontrol (| ls)
	if (t->type == TOKEN_PIPE)
		return (print_syntax_error(t->value));
	
	while (t)
	{
		if (t->type == TOKEN_PIPE)
		{
			// Bir sonraki tokenın Pipe olup olmadığını kontrol et (| |) veya NULL durumu (ls |)
			if (!t->next)
				return (print_syntax_error(NULL));
			if (t->next->type == TOKEN_PIPE)
				return (print_syntax_error(t->next->value));
		}
		t = t->next;
	}
	return (0);
}

static int	check_redir_syntax(t_token *t)
{
	while (t)
	{
		if (is_redir_token(t->type))
		{
			// Sonrakini var olduğunu ve operatör olduğunu kontrol et
			// bash: syntax error near unexpected token `>' or `|'
			if (!t->next)
				return (print_syntax_error(NULL));
			if (t->next->type != TOKEN_WORD) // Eğer sıradaki operator ise(PIPE or REDIR)
				return (print_syntax_error(t->next->value));
		}
		t = t->next;
	}
	return (0);
}

int	validate_tokens(t_sh *shell)
{
	if (!shell->token_list)
		return (0); // Boş olması geçerlidir veya okuyucu tarafından ele alınır
	if (check_pipe_syntax(shell->token_list))
	{
		shell->exit_status = 2; // Bash sözdizimi hata kodu 2'dir
		return (1);
	}
	if (check_redir_syntax(shell->token_list))
	{
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}
