#include "../includes/WCMS.h"

static void    check_quote(size_t *size, char *value, t_lexer_state *state)
{
	if (value[*size] == '\"' && !state->in_single_quote)
    {
		state->in_double_quote = !state->in_double_quote;
        size++;
    }
	if (value[*size] == '\'' && !state->in_double_quote)
    {
		state->in_single_quote = !state->in_single_quote;
        size++;
    }
}

static size_t  new_size_of_value(t_lexer_state state, char *value)
{
    size_t  size;
    
    size = 0;
    if (value[size] == '\"')
		state.in_double_quote = 1;
	else
		state.in_single_quote = 1;
	(size)++;
	while (value[size])
    {
        check_quote(&size, value, &state);
        size++;
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
    temp = malloc(sizeof(char) * (size + 1));
    if (!temp)
        return (NULL);
    if (value[i] == '\"')
		state.in_double_quote = 1;
    else
		state.in_single_quote = 1;
    i++;
    size = 0;
	while (value[i])
	{
        check_quote(&i, value, &state);
        temp[size] = value[i];
		i++;
        size++;
	}
    printf("merhababbaaaaaaaaa\n");
    temp[size] = '\0';
    return (temp);
}

int expander(t_token **tokens)
{
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAĞĞĞĞ\n");
    char *new_value;
    t_token *temp = *tokens;
    while (temp)
    {
        printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBĞĞ\n");
        if (temp->type == TOKEN_WORD)
        {
            printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCĞĞ\n");
            printf("%d\n", temp->quote_type);

            if (temp->quote_type != N_QUOTE)
            {
                new_value = create_new_value(temp->value);
                free(temp->value);
                temp->value = new_value;
            }
        }   
        temp = temp->next;
    }
    return (0);
}
