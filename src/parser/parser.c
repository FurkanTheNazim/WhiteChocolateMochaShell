/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 20:10:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/02/17 21:22:56 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

int	is_redir_token(int type);
t_token	*skip_to_pipe(t_token *token);

static void	handle_redirection(t_sh *shell, t_command *cmd, t_token **token)
{
	t_redir	*redir;
	int		type;
	char	*file;

	type = (*token)->type;
	if ((*token)->next)
	{
		file = (*token)->next->value; // 'value' (tırnaksız) mı yoksa 'raw' (ham) mı kullanılmalı? Genellikle 'value'.
		// Not: Genişletmeyi (expansion) burada veya daha sonra ele almamız gerekebilir.
		// Şimdilik 'value'nun lexer veya expander ön geçişi tarafından hazırlandığını varsayıyoruz.
		// Kontrol listesi, tırnak temizlemenin daha sonra veya daha önce yapılabileceğini belirtiyor.
		// Eğer lexer tırnakları hallettiyse tokendan gelen 'value'yu kullanmak en güvenlisi.
	}
	else
		file = NULL; // Sözdizimi hatası durumu
	
	redir = create_redir(shell, type, file);
	add_redir(shell, cmd, redir);
	
	// Tokenı ilerlet: REDIR_OP -> FILE -> [Next]
	*token = (*token)->next;
	if (*token)
		*token = (*token)->next;
}

static t_command	*parse_simple_command(t_sh *shell, t_token **token)
{
	t_command	*cmd;

	cmd = create_command(shell);
	if (!cmd)
		return (NULL);
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (is_redir_token((*token)->type))
		{
			handle_redirection(shell, cmd, token);
		}
		else
		{
			// Bu bir kelime/argüman
			add_arg(shell, cmd, (*token)->value);
			*token = (*token)->next;
		}
	}
	return (cmd);
}

t_command	*parser(t_sh *shell)
{
	t_command	*cmd_list;
	t_command	*new_cmd;
	t_token		*token;

	if (!shell || !shell->token_list)
		return (NULL);
	
	token = shell->token_list;
	cmd_list = NULL;
	
	if (validate_tokens(shell))
		return (NULL);

	while (token)
	{
		new_cmd = parse_simple_command(shell, &token);
		add_command(shell, &cmd_list, new_cmd);
		
		if (token && token->type == TOKEN_PIPE)
			token = token->next;
	}
	shell->command_list = cmd_list; // Shell yapısında sakla
	return (cmd_list);
}
