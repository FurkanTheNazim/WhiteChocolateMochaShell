	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 17:00:10 by mahmmous          #+#    #+#             */
/*   Updated: 2026/04/06 17:28:59 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/WCMS.h"

t_ast_node	*create_ast_cmd(t_sh *sh, t_command *cmd)
{
	t_ast_node	*node;

	node = gc_malloc(sh, sizeof(t_ast_node), 0);
	if (!node)
		return (NULL);
	node->type = AST_CMD;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node	*create_ast_pipe(t_sh *sh, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = gc_malloc(sh, sizeof(t_ast_node), 0);
	if (!node)
		return (NULL);
	node->type = AST_PIPE;
	node->cmd = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

static t_ast_node	*list_to_ast(t_sh *sh, t_command *cmd)
{
	t_ast_node	*left;
	t_ast_node	*right;

	if (!cmd)
		return (NULL);
	if (!cmd->next)
		return (create_ast_cmd(sh, cmd));
	left = create_ast_cmd(sh, cmd);
	right = list_to_ast(sh, cmd->next);
	return (create_ast_pipe(sh, left, right));
}

t_ast_node	*build_ast(t_sh *sh)
{
	t_command	*cmd_list;

	cmd_list = parser(sh);
	if (!cmd_list)
		return (NULL);
	sh->ast = list_to_ast(sh, cmd_list);
	return (sh->ast);
}

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		ft_putstr_fd("  ", 1);
		i++;
	}
}

void	print_ast(t_ast_node *node, int depth)
{
	int	i;

	if (!node)
		return ;
	print_indent(depth);
	if (node->type == AST_PIPE)
	{
		ft_putendl_fd("[PIPE]", 1);
		print_ast(node->left, depth + 1);
		print_ast(node->right, depth + 1);
	}
	else if (node->type == AST_CMD)
	{
		ft_putstr_fd("[CMD] ", 1);
		if (node->cmd && node->cmd->args)
		{
			i = 0;
			while (node->cmd->args[i])
			{
				ft_putstr_fd(node->cmd->args[i], 1);
				ft_putstr_fd(" ", 1);
				i++;
			}
		}
		if (node->cmd && node->cmd->redirs)
		{
			t_redir *tmp = node->cmd->redirs;
			ft_putstr_fd(" | Redirs: ", 1);
			while (tmp)
			{
				ft_putstr_fd(tmp->file, 1);
				ft_putstr_fd(" ", 1);
				tmp = tmp->next;
			}
		}
		ft_putstr_fd("\n", 1);
	}
}
