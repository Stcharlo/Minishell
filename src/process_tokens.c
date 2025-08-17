/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:21:44 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 12:46:29 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	has_heredoc(t_token *lst)
{
	while (lst)
	{
		if (lst->type == HEREDOC)
			return (1);
		lst = lst->next;
	}
	return (0);
}

/// @brief parsing user input
/// @param lst list for the tokens
/// @param line input from the user
/// @param env
static int	init_tokens_and_check(t_token **lst, char *line, t_ast **env,
		t_token_info **tokens)
{
	int	token_count;

	line = unquoted_var_expansion(line, env);
	*tokens = split_bash_style_with_quotes(line, &token_count);
	*lst = NULL;
	create_list_with_quote_info(lst, *tokens, token_count);
	check_heredoc(lst, env);
	if (has_heredoc(*lst) && g_exit_code == 130)
	{
		(*env)->env->error_code = 130;
		free_token_info_array(*tokens, token_count);
		free_stack(lst);
		return (0);
	}
	if (check_syntax_errors(*lst))
	{
		(*env)->env->error_code = 2;
		free_token_info_array(*tokens, token_count);
		free_stack(lst);
		return (0);
	}
	return (1);
}

static void	handle_ast_and_exec(t_token **lst, t_ast **env,
		t_token_info *tokens)
{
	ASTNode	**nodes;

	exit_status(lst, env);
	nodes = build_and_print_ast(*lst, env);
	if (nodes && *nodes)
	{
		execute_nodes(nodes, env);
		ast_free(*nodes);
	}
	if (nodes)
		free(nodes);
	unlink_redirection(lst);
	free_token_info_array(tokens, ft_lstsize(*lst));
	free_stack(lst);
}

void	process_tokens(t_token **lst, char *line, t_ast **env)
{
	t_token_info	*tokens;

	if (!init_tokens_and_check(lst, line, env, &tokens))
		return ;
	handle_ast_and_exec(lst, env, tokens);
}
