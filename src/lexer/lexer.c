#include "../includes/WCMS.h"

t_token	*create_newnode(char *token)
{
	t_token	*node;

	if (!token)
		return (NULL);
	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->content = token;
	if (*token == '|')
		node->type = TOKEN_PIPE;
	else if (token[0] == '<' && token[1] == '<')
		node->type = TOKEN_HEREDOC;
	else if (token[0] == '>' && token[1] == '>')
		node->type = TOKEN_REDIR_APPEND;
	else if (*token == '>')
		node->type = TOKEN_REDIR_OUT;
	else if (*token == '<')
		node->type = TOKEN_REDIR_IN;
	else
		node->type = TOKEN_WORD;
	node->next = NULL;
	return (node);
}
void	addback(t_token **node, t_token *newnode)
{
	t_token	*temp;
	
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
int lexer_operator(t_token **list, char *input)
{
	char *tmp;
	int len;

	len = 1;
	if((input[0] == '>' && input[1] == '>') ||
		(input[0] == '<' && input[1] == '<'))
		len = 2;
	tmp = ft_substr(input, 0, len);
	addback(list, create_newnode(tmp));
	return (len);	
}

int	lexer_double_quote(t_token **list, char *input)
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

int	lexer_quote(t_token **list, char *input)
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

t_token	*lexer(char *input)
{
	t_token	*tokenlist = NULL;
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
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			j += lexer_operator(&tokenlist, &input[i]);
		else
		{
			while (input[i + j] && input[i + j] != ' ' 
					&& input[i + j] != '|' && input[i + j] != '<' && input[i + j] != '>')
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
	t_token	*tokenlist;
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
		t_token *temp;
		temp = tokenlist;
		while(temp)
		{
			printf("i: %d || type[%d] : %s\n",i, temp->type, temp->content);
			temp = temp->next;
			i++;
		}
	}
	return 0;
}