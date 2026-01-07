#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFT 1
#define RIGHT 2

typedef struct s_tree
{
	char type;
	char **cmd;
	char *path;
	t_tree *left;
	t_tree *right;

}	t_tree;

t_tree *create_new_node(char type, char **cmd, char *path)
{
	t_tree *node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = cmd;
	node->path = path;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	addlist(int pos, t_tree *node, t_tree *new_node)
{
	if (pos == LEFT)
		node->left = new_node;
	else if (pos == RIGHT)
		node->right = new_node;
}

void create_tree(int ac, char **av)
{
	
}
