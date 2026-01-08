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