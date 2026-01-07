/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_house.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 08:36:36 by mahmmous          #+#    #+#             */
/*   Updated: 2026/01/02 21:40:48 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

typedef struct s_tree
{
	int				data;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

t_tree	*new_node(int data)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_tree	*insert_node(t_tree *root, int data)
{
	if (!root)
		return (new_node(data));
	if (data < root->data)
		root->left = insert_node(root->left, data);
	else
		root->right = insert_node(root->right, data);
	return (root);
}

void	inorder(t_tree *root)
{
	if (!root)
		return ;
	inorder(root->left);
	printf("%d ", root->data);
	inorder(root->right);
}

int	main(int argc, char **argv)
{
	t_tree	*root;
	int		i;

	if (argc < 2)
	{
		printf("Usage: ./tree_house num1 num2 num3 ...\n");
		return (1);
	}
	root = NULL;
	i = 1;
	while (i < argc)
	{
		root = insert_node(root, atoi(argv[i]));
		i++;
	}
	printf("Inorder: ");
	inorder(root);
	printf("\n");
	return (0);
}
