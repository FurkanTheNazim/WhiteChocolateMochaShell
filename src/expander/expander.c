#include "../includes/WCMS.h"

int expander(t_token **tokens)
{
    int i = 0;
    char *new_value;
    t_token *temp = *tokens;
    while (temp)
    {
        if (temp->type == TOKEN_WORD)
        {
            if (temp->quote_type != N_QUOTE)
            {
                new_value = ft_strtrim(temp->value, "\'\"");
                free(temp->value);
                temp->value = new_value;
            }
            if (ft_strchr(temp->value, '$') && temp->quote_type != S_QUOTE)
            {
                
            }
        }   
        temp = temp->next;
    }
    return (0);
}

int main (int argc, char **argv)
{
    int i;

    i = 0;
    
}   