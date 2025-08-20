/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:04:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/20 13:45:31 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	define_type(char *str, t_ast **env)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(str, env);
	if (cmd_path)
	{
		free(cmd_path);
		return (NODE_COMMAND);
	}
	if (!strcmp(str, "<<") || !strcmp(str, "<") || !strcmp(str, ">>")
		|| !strcmp(str, ">"))
		return (NODE_REDIRECTION);
	if (!strcmp(str, "|"))
		return (NODE_PIPE);
	return (NODE_ARGUMENT);
}

void	ast_free(ASTNode *node)
{
    int	i;

    if (!node)
        return ;
    printf("Freeing node: %p, value: %s\n", (void*)node, node->value); // Debug print
    free(node->value);
    for (i = 0; i < node->child_count; i++)
        ast_free(node->children[i]);
    free(node->children);
    if (node->left)
        ast_free(node->left);
    if (node->right)
        ast_free(node->right);
    if (node->target)
        ast_free(node->target);
    free(node);
}

void	set_ast_left(ASTNode *parent, ASTNode *child)
{
	parent->left = child;
	if (child)
		child->parent = parent;
}

void	set_ast_right(ASTNode *parent, ASTNode *child)
{
	parent->right = child;
	if (child)
		child->parent = parent;
}

void	add_redirection(ASTNode *cmd, ASTNode *redir)
{
	add_ast_child(cmd, redir);
}
