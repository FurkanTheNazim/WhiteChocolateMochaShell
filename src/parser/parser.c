/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   parser.c                                             :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/02/10 20:10:00 by mahmmous            #+#    #+#           */
/*   Updated: 2026/05/28 04:51:41 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

int		is_redir_token(int type);
t_token	*skip_to_pipe(t_token *token);

static void	handle_redirection(t_sh *shell, t_command *cmd, t_token **token)
{
	t_redir	*redir;
	int		type;
	char	*file;

	type = (*token)->type;
	file = (*token)->next->value;
	redir = create_redir(shell, type, file);
	add_redir(shell, cmd, redir);
	*token = (*token)->next->next;
}

static t_command	*parse_simple_command(t_sh *shell, t_token **token)
{
	t_command	*cmd;

	cmd = create_command(shell);
	if (!cmd)
		return (NULL);
	while (*token && (*token)->type != TOKEN_PIPE && (*token)->type != TOKEN_OR)
	{
		if (is_redir_token((*token)->type))
		{
			handle_redirection(shell, cmd, token);
		}
		else if ((*token)->type == TOKEN_HEREDOC)
		{
			(*token) = (*token)->next->next;
		}
		else
		{
			add_arg(shell, cmd, *token);
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
	while (token)
	{
		new_cmd = parse_simple_command(shell, &token);
		if (new_cmd && new_cmd->args && new_cmd->args[0])
			new_cmd->builtin = identify_builtin(new_cmd->args[0]);
		add_command(shell, &cmd_list, new_cmd);
		if (token && (token->type == TOKEN_PIPE || token->type == TOKEN_OR))
			token = token->next;
	}
	shell->command_list = cmd_list;
	return (cmd_list);
}
