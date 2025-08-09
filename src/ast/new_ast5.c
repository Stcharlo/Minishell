/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:19:50 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 11:20:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	attach_redirections(ASTNode *cmd, t_token *lst, t_ast **env)
{
	t_token	*cur;
	t_token	*redir_token;
	ASTNode	*redir_node;
	ASTNode	*target_node;

	cur = lst;
	while (cur && define_type(cur->value, env) != NODE_PIPE)
	{
		if (define_type(cur->value, env) == NODE_REDIRECTION)
		{
			redir_token = cur;
			redir_node = create_ast_node(NODE_REDIRECTION, redir_token->value);
			cur = cur->next;
			target_node = NULL;
			if (cur && define_type(cur->value, env) != NODE_PIPE
				&& define_type(cur->value, env) != NODE_REDIRECTION)
				target_node = create_ast_node(NODE_TARGET, cur->value);
			set_redirection_target(redir_node, target_node);
			add_ast_child(cmd, redir_node);
		}
		cur = cur->next;
	}
}

ASTNode	*parse_pipeline(t_token **lst_ptr, t_ast **env)
{
	ASTNode	*left;
	ASTNode	*pipe;
	ASTNode	*right;
	t_token	*lst;
	t_token	*cur;

	lst = *lst_ptr;
	left = parse_command(lst_ptr, env);
	attach_redirections(left, lst, env);
	cur = *lst_ptr;
	if (cur && define_type(cur->value, env) == NODE_PIPE)
	{
		pipe = create_ast_node(NODE_PIPE, cur->value);
		set_ast_left(pipe, left);
		*lst_ptr = cur->next;
		right = parse_pipeline(lst_ptr, env);
		set_ast_right(pipe, right);
		return (pipe);
	}
	return (left);
}
