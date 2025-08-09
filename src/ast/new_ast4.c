/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:14:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 11:20:20 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	define_type_from_token(t_token *token, t_ast **env)
{
	char	*cmd_path;

	if (token->was_quoted && (!strcmp(token->value, "|")
			|| !strcmp(token->value, "<") || !strcmp(token->value, ">")
			|| !strcmp(token->value, ">>") || !strcmp(token->value, "<<")))
		return (NODE_ARGUMENT);
	cmd_path = get_cmd_path(token->value, env);
	if (cmd_path)
	{
		free(cmd_path);
		return (NODE_COMMAND);
	}
	if (!strcmp(token->value, "<<") || !strcmp(token->value, "<")
		|| !strcmp(token->value, ">>") || !strcmp(token->value, ">"))
		return (NODE_REDIRECTION);
	if (!strcmp(token->value, "|"))
		return (NODE_PIPE);
	return (NODE_ARGUMENT);
}

void	set_redirection_target(ASTNode *redir, ASTNode *target)
{
	redir->target = target;
}

static ASTNode	*parse_command_head(t_token **lst_ptr, t_ast **env)
{
	t_token	*lst;
	ASTNode	*cmd;

	lst = *lst_ptr;
	cmd = NULL;
	while (lst && define_type_from_token(lst, env) != NODE_PIPE)
	{
		if (define_type_from_token(lst, env) != NODE_REDIRECTION)
		{
			cmd = create_ast_node(NODE_COMMAND, lst->value);
			lst = lst->next;
			break ;
		}
		lst = lst->next;
		if (lst)
			lst = lst->next;
	}
	*lst_ptr = lst;
	return (cmd);
}

static void	parse_command_args(ASTNode *cmd, t_token **lst_ptr, t_ast **env)
{
	t_token	*lst;

	lst = *lst_ptr;
	while (lst && define_type_from_token(lst, env) != NODE_PIPE)
	{
		if (define_type_from_token(lst, env) != NODE_REDIRECTION)
		{
			add_ast_child(cmd, create_ast_node(NODE_ARGUMENT, lst->value));
			lst = lst->next;
		}
		else
		{
			lst = lst->next;
			if (lst)
				lst = lst->next;
		}
	}
	*lst_ptr = lst;
}

ASTNode	*parse_command(t_token **lst_ptr, t_ast **env)
{
	ASTNode	*cmd;

	cmd = parse_command_head(lst_ptr, env);
	parse_command_args(cmd, lst_ptr, env);
	return (cmd);
}
