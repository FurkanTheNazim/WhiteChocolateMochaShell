#include "../includes/WCMS.h"

#define COMMAND 1
#define PIPE 2
#define REDIRECTION 3

typedef struct s_token_list
{
	int				type;
	char			*token;
	void			*next;
}	t_token_list;


t_token_list	*create_newnode(char *token)
{
	t_token_list	*node;

	if (!token)
		return (NULL);
	node = malloc(sizeof(t_token_list));
	if (!node)
		return (NULL);
	node->token = token;
	if (*token == '|')
		node->type = PIPE;
	else if (*token == '<' || *token == '>')
		node->type = REDIRECTION;
	else
		node->type = COMMAND;
	node->next = NULL;
	return (node);
}

void	addback(t_token_list **node, t_token_list *newnode)
{
	t_token_list	*temp;

	if (!*node)
	{
		*node = newnode;
		return ;
	}
	temp = *node;
	while (temp->next)
		temp = temp->next;
	temp->next = newnode;
}

int	lexer_double_quote(t_token_list **list, char *input)
{
	char	*tmp;
	int		i;

	i = 1;
	while (input[i] && input[i] != '\"')
		i++;
	if (input[i] == '\"')
		i++;
	tmp = ft_substr(input, 0, i);
	addback(list, create_newnode(tmp));
	return (i);
}

int	lexer_quote(t_token_list **list, char *input)
{
	char	*tmp;
	int		i;

	i = 1;
	while (input[i] && input[i] != '\'')
		i++;
	if (input[i] == '\'')
		i++;
	tmp = ft_substr(input, 0, i);
	addback(&(*list), create_newnode(tmp));
	return (i);
}

t_token_list	*lexer(char *input)
{
	t_token_list	*tokenlist;
	char			*temp;
	int		i;
	int		j;

	i = 0;
	while (input[i])
	{
		j = 0;
		while (input[i] && input[i] == ' ')
			i++;		
		if (input[i] && input[i] == '\"')
			j += lexer_double_quote(&tokenlist, &input[i]);
		else if (input[i] && input[i] == '\'')
			j += lexer_quote(&tokenlist, &input[i]);
		else
		{
			while (input[i + j] && input[i + j] != ' ')
				j++;
			temp = ft_substr(input, i, j);
			addback(&tokenlist, create_newnode(temp));
		}
		i += j;
	}
	return (tokenlist);
}

int main()
{
	t_token_list	*tokenlist;
	char			*input;
	int				rvalue;
	int i = 0;

	rvalue = 0;
	while (1)
	{
		input = readline("mochashell>");
		if (!input)
		{
			rl_clear_history();
			free(input);
			return (rvalue);
		}
		add_history(input);
		tokenlist = lexer(input);
		free(input);
		t_token_list *temp;
		temp = tokenlist;
		while(temp)
		{
			printf("i: %d || type[%d] : %s\n",i, temp->type, temp->token);
			temp = temp->next;
			i++;
		}
	}
	return 0;
}