
// static int    check_quote(size_t size, char *value, t_lexer_state *state)
// {
// 	if (value[size] == '\"' && !state->in_single_quote)
//     {
// 		state->in_double_quote = !state->in_double_quote;
//         size++;
//     }
// 	if (value[size] == '\'' && !state->in_double_quote)
//     {
// 		state->in_single_quote = !state->in_single_quote;
//         size++;
//     }
//     return (size);
// }

// static size_t  new_size_of_value(t_lexer_state state, char *value)
// {
//     size_t  size;
//     size_t     i;

//     size = 0;
//     i = 0;
//     if (value[size] == '\"')
// 		state.in_double_quote = 1;
// 	else
// 		state.in_single_quote = 1;
// 	i++;
// 	while (value[i])
//     {
//         i = check_quote(i, value, &state);
//         size++;
//         i++;
//     }
//     return (size);
// }

// static char *create_new_value(char *value)
// {
//     t_lexer_state   state;
//     size_t          size;
//     char            *temp;
//     size_t             i;

//     i = 0;
//     size = new_size_of_value(state, value);
//     init_lexer_stat(&state);
//     temp = gc_malloc(sizeof(char) * size);
//     if (!temp)
//         return (NULL);
//     if (value[i++] == '\"')
// 		state.in_double_quote = 1;
//     else
// 		state.in_single_quote = 1;
//     size = 0;
// 	while (value[i])
// 	{
//         i = check_quote(i, value, &state);
//         temp[size] = value[i];
// 		i++;
//         size++;
// 	}
//     temp[size] = '\0';
//     return (temp);
// }

int expander(t_sh *shell)
{
    char *new_value;
	char *tmp_str;
    int size = 0;
    int start = 0;
    int i;
    t_token *temp = shell->token_list;
    while (temp)
    {
        if (temp->type == TOKEN_WORD)
        {
            while(temp->value[i])
            {
				// if (temp->value[i] == "" || '' && size != 0)
				// {
				// 	new_value = ft_substr(temp->value, start, size);
				// 	start += size;
				// }
				if (value[i] == " " && !ins_q)
					ind = 1;
					size = i;
				else if (value == '' && !inD)
					size = i;
				if (size != start)
					tmp = substr(value,start, size -1);
					inD = 0;
					inS = 0;
					start = i + 1;
				if (!tmp_str && tmp_str[0] != '' && strchr(tmp_str, $))
					tmp_str = expander(tmp_str);
				if (!tmp_str)
					trim(tmp_str);,
					join(new_value, tmp_str);
					free(tmp_str);
				i++;
            }
			free(eski value);
			eskivalue= new_value;
            // printf("qtype:%d\n", temp->quote_type);
            // new_value = create_new_value(temp->value);
            // free(temp->value);
            // temp->value = new_value;
            // printf("new value %s\n", temp->value);
        }  
        temp = temp->next;
    }
    return (0);
}

