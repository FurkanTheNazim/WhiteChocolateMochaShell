#include <unistd.h>
#include <stdio.h>
#include "includes/WCMS.h"

int main(void)
{
	char *s = "";
	char **tmp;

	tmp = ft_split(s, ' ');
	if (!tmp)
	{
		printf("NULL geldi\n");
		return 0;
	}
	printf("[%s]\n", tmp[0]);

}