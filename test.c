#include "includes/WCMS.h"

void	test(t_token *tmp)
{
	tmp->value = "görüşürüz";
}

int main()
{
	t_token *tmp;

	tmp = malloc(sizeof(t_token));
	ft_bzero(tmp, sizeof(t_token));
	perror("1");
	tmp->value =ft_strdup("selam");
	test(tmp);
	ft_printf("%s\n", tmp->value);
	free(tmp);
}