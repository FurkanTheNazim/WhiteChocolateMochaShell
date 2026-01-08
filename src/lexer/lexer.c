#include "WCMS.h"


void	init_token(t_minishell *main)
{
	main->token_list->raw = main->input;
	main->token_list->quote_type = 0;
	main->token_list->type = 0;	
}

t_token	*create_newnode(char *raw, t_lexer_state state, char *token)
{
	t_token	*node;

	if (!token)
		return (NULL);
	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->raw = raw;//buna gerek olmayabilir
	node->value = token;
	if (state.in_double_quote)
		node->quote_type = D_QUOTE;
	else if (state.in_single_quote)
		node->quote_type = S_QUOTE;
	else
		node->quote_type = N_QUOTE;
	node->next = NULL;
	return (node);
}


void	addback(t_minishell *main, t_token *newnode)
{
	t_token	*temp;

	if (!main->token_list)
	{
		main->token_list = newnode;
		return ;
	}
	temp = main->token_list;
	while (temp->next)
		temp = temp->next;
	temp->next = newnode;
}

int	append_operator(t_lexer_state state, char *ptr, t_minishell *main)
{
	int	i;

	i = 0;
	if (ptr[i] == '|')
	{
		if (ptr[++i] == '|')
		{
			main->token_list->type = TOKEN_OR;
			i++;
		}
		else
			main->token_list->type = TOKEN_PIPE;
	}
	else if (ptr[i] == '<')
	{
		if (ptr[++i] == '<')
		{
			main->token_list->type = TOKEN_HEREDOC;
			i++;
		}
		else
			main->token_list->type = TOKEN_REDIR_IN;
	}
	else if (ptr[i] == '>')
	{
		if (ptr[++i] == '>')
		{
			main->token_list->type = TOKEN_REDIR_APPEND;
			i++;
		}
		else
			main->token_list->type = TOKEN_REDIR_OUT;
	}
	addback(main, create_newnode(main->token_list->raw, state, ft_substr(ptr, 0, i)));
	return  (i);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	ft_isspace(char c)
{
	return (c == 9 || c == 32); 
}

void	init_lexer_stat(t_lexer_state *state)
{
	state->in_double_quote = 0;
	state->in_single_quote = 0;
}

t_token	*lexer(t_minishell *main)
{
	t_lexer_state	state;
	int				i;
	int				j;

	i = 0;
	while (main->input[i])
	{
		j = 0;
		init_lexer_stat(&state);
		while (main->input[i] && ft_isspace(main->input[i]))
			i++;
		if (is_operator(main->input[i]))
			j += append_operator(state, &main->input[i], main);
		else
		{
			while (main->input[i + j] && !ft_isspace(main->input[i + j]) && !is_operator(main->input[i + j]))
			{
				if (main->input[i + j] == '\"' && !state.in_single_quote)
				{
					state.in_double_quote = 1;
					while (main->input[i + j] != '\"' && !state.in_single_quote)
					j++;
					j++;
				}
				else if (main->input[i] == '\'' && !state.in_double_quote)
				{
					state.in_single_quote = 1;
					while (main->input[i + j] != '\'' && !state.in_double_quote)
					j++;
				}
				else
				j++;
			}
			addback(main ,create_newnode(main->input, state ,ft_substr(main->input, i, j)));
		}
		i += j;
	}
	return (main->token_list);
}

//  merhaba> "furkan kerem kuma'mız ayşe" can çekişiyor.
//merhaba
//>
// furkan ...

void	free_node(t_token **list)
{
	t_token	*temp;

	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
			free((*list)->value);
		if ((*list)->raw)
			free((*list)->raw);
		free(*list);
		*list = temp;
	}
	*list = NULL;
}

int main()
{
	t_minishell		main;

	int i = 0;
 	main.exit_status = 0; //daha sonra hepsi birlikte initialize edilir
	while (1)
	{
		main.input = readline("mochashell>");
		if (!main.input)
		{
			rl_clear_history();
			free(main.input);
			return (main.exit_status);
		}
		add_history(main.input);
		main.token_list = lexer(&main);
		t_token *temp;
		temp = main.token_list;
		while(temp)
		{
			printf("i: %d || type[%d] : %s\n",i, temp->type, temp->value);
			temp = temp->next;
			i++;
		}
	}
	return 0;
}
