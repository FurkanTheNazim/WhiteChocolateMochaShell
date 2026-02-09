#include "../includes/lexer_parser.h"

t_command *create_command(void)
{
    t_command *new = (t_command *)gc_malloc(sizeof(t_command));
    if (!new)
        return NULL;
    new->args = NULL;
    new->redirs = NULL;
    new->next = NULL;
    return new;
}
