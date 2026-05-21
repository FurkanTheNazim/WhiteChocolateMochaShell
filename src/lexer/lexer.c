/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   lexer.c                                              :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/01/09 18:00:00 by minishell           #+#    #+#           */
/*   Updated: 2026/05/15 01:30:12 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

static int	handle_double_quote(char *input, int *j, t_lexer *state)
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

static int	handle_single_quote(char *input, int *j, t_lexer *state)
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

static int	handle_word(t_sh *sh, int i, t_lexer *state)
{
	int		j;
	char	*input;

	j = 0;
	input = sh->input;
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
	if (addback(sh, newnode(sh, sh->input, ft_substr(sh->input, i, j))) < 0)
		return (-2);
	return (j);
}

static int	handle_syntax_error(t_sh *sh)
{
	printf("minishell: syntax error: unclosed quote\n");
	sh->syntax_error = 1;
	return (1);
}

t_token	*lexer(t_sh *sh)
{
	t_lexer	state;
	int				i;
	int				j;

	i = 0;
	while (sh->input[i])
	{
		init_lexer_stat(&state);
		while (sh->input[i] && ft_isspace(sh->input[i]))
			i++;
		if (!sh->input[i])
			break ;
		if (is_operator(sh->input[i]))
			j = append_operator(&sh->input[i], sh);
		else
			j = handle_word(sh, i, &state);
		if (j == -2)
			return (NULL);
		if (j == -1)
			return (handle_syntax_error(sh), NULL);
		i += j;
	}
	return (sh->token_list);
}
