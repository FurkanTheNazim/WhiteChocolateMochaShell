static int	update_quote_status()
{
	if (input[*j] == '\"')
		state->in_double_quote = 1;
	else
		state->in_single_quote = 1;
	(*j)++;
	while (input[*j] && !ft_isspace(input[*j]))
	{
		if (input[*j] == '\"' && !state->in_single_quote)
			state->in_double_quote = !state->in_double_quote;
		if (input[*j] == '\'' && !state->in_double_quote)
			state->in_single_quote = !state->in_single_quote;
		(*j)++;
	}
	if (state->in_double_quote || state->in_single_quote)
		return (1);
	(*j)++;
	return (0);
}