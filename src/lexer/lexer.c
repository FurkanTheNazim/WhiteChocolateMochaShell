/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:00:00 by minishell         #+#    #+#             */
/*   Updated: 2026/01/09 18:02:01 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static void	handle_double_quote(char *input, int *j, t_lexer_state *state)
{
	state->in_double_quote = 1;
	(*j)++;
	while (input[*j] && input[*j] != '\"')
		(*j)++;
	if (input[*j] == '\"')
		(*j)++;
}

static void	handle_single_quote(char *input, int *j, t_lexer_state *state)
{
	state->in_single_quote = 1;
	(*j)++;
	while (input[*j] && input[*j] != '\'')
		(*j)++;
	if (input[*j] == '\'')
		(*j)++;
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
			handle_double_quote(&input[i], &j, state);
		else if (input[i + j] == '\'' && !state->in_double_quote)
			handle_single_quote(&input[i], &j, state);
		else
			j++;
	}
	addback(shell, create_newnode(shell->input, *state,
			ft_substr(shell->input, i, j)));
	return (j);
}

t_token	*lexer(t_minishell *shell)
{
	t_lexer_state	state;
	int				i;
	int				j;

	i = 0;
	while (shell->input[i])
	{
		j = 0;
		init_lexer_stat(&state);
		while (shell->input[i] && ft_isspace(shell->input[i]))
			i++;
		if (!shell->input[i])
			break ;
		if (is_operator(shell->input[i]))
			j = append_operator(state, &shell->input[i], shell);
		else
			j = handle_word(shell, i, &state);
		i += j;
	}
	return (shell->token_list);
}
