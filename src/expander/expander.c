#include "../includes/WCMS.h"

static int    check_quote(size_t size, char *value, t_lexer_state *state)
{
	if (value[size] == '\"' && !state->in_single_quote)
    {
		state->in_double_quote = !state->in_double_quote;
        size++;
    }
	if (value[size] == '\'' && !state->in_double_quote)
    {
		state->in_single_quote = !state->in_single_quote;
        size++;
    }
    return (size);
}

static size_t  new_size_of_value(t_lexer_state state, char *value)
{
    size_t  size;
    size_t     i;

    size = 0;
    i = 0;
    if (value[size] == '\"')
		state.in_double_quote = 1;
	else
		state.in_single_quote = 1;
	i++;
	while (value[i])
    {
        i = check_quote(i, value, &state);
        size++;
        i++;
    }
    return (size);
}

static char *create_new_value(char *value)
{
    t_lexer_state   state;
    size_t          size;
    char            *temp;
    size_t             i;

    i = 0;
    size = new_size_of_value(state, value);
    init_lexer_stat(&state);
    temp = malloc(sizeof(char) * size);
    if (!temp)
        return (NULL);
    if (value[i++] == '\"')
		state.in_double_quote = 1;
    else
		state.in_single_quote = 1;
    size = 0;
	while (value[i])
	{
        i = check_quote(i, value, &state);
        temp[size] = value[i];
		i++;
        size++;
	}
    temp[size] = '\0';
    return (temp);
}

int expander(t_token **tokens)
{
    char *new_value;
    t_token *temp = *tokens;
    while (temp)
    {
        if (temp->type == TOKEN_WORD)
        {
            printf("%d\n", temp->quote_type);
            new_value = create_new_value(temp->value);
            free(temp->value);
            temp->value = new_value;
            printf("new value %s\n", temp->value);
        }  
        temp = temp->next;
    }
    return (0);
}
