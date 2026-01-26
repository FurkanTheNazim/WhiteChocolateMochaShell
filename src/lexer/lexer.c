/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/01/10 08:28:10 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static int	handle_double_quote(char *input, int *j, t_lexer_state *state)
{
	state->in_double_quote = 1;
	(*j)++;
	while (input[*j] && input[*j] != '\"')
		(*j)++;
	if (!input[*j])
		return (1);
	(*j)++;
	state->in_double_quote = 0;
	return (0);
}

static int	handle_single_quote(char *input, int *j, t_lexer_state *state)
{
	state->in_single_quote = 1;
	(*j)++;
	while (input[*j] && input[*j] != '\'')
		(*j)++;
	if (!input[*j])
		return (1);
	(*j)++;
	state->in_single_quote = 0;
	return (0);
}

static int	handle_word(t_minishell *shell, int i, t_lexer_state *state)
{
	int		j;
	char	*input;

	j = 0;
	input = shell->input;
	while (input[i + j] && !ft_isspace(input[i + j])
		&& !is_operator(input[i + j]))
	{
		if (input[i + j] == '\"' && !state->in_single_quote)
		{
			if (handle_double_quote(&input[i], &j, state))
				return (-1);
		}
		else if (input[i + j] == '\'' && !state->in_double_quote)
		{
			if (handle_single_quote(&input[i], &j, state))
				return (-1);
		}
		else
			j++;
	}
	addback(shell, create_newnode(shell->input, *state,
			ft_substr(shell->input, i, j)));
	return (j);
}

static int	handle_syntax_error(t_minishell *shell)
{
	printf("minishell: syntax error: unclosed quote\n");
	shell->syntax_error = 1;
	return (1);
}

t_token	*lexer(t_minishell *shell)
{
	t_lexer_state	state;
	int				i;
	int				j;

	i = 0;
	while (shell->input[i])
	{
		init_lexer_stat(&state);
		while (shell->input[i] && ft_isspace(shell->input[i]))
			i++;
		if (!shell->input[i])
			break ;
		if (is_operator(shell->input[i]))
			j = append_operator(state, &shell->input[i], shell);
		else
			j = handle_word(shell, i, &state);
		if (j == -1)
			return (handle_syntax_error(shell), NULL);
		i += j;
	}
	return (shell->token_list);
}
