/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:46:48 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 15:41:10 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Check for redirection without a filename
 *
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
static int	is_redirection_token(t_token *token)
{
	return (token->type == INPUT_REDIRECT || token->type == OUTPUT_REDIRECT
		|| token->type == APPEND || token->type == HEREDOC
		|| (!token->was_quoted && (!ft_strcmp(token->value, "<")
				|| !ft_strcmp(token->value, ">") || !ft_strcmp(token->value,
					">>") || !ft_strcmp(token->value, "<<"))));
}

static int	is_invalid_next_token(t_token *token)
{
	return (token->type == PIPE || token->type == INPUT_REDIRECT
		|| token->type == OUTPUT_REDIRECT || token->type == APPEND
		|| token->type == HEREDOC || (!token->was_quoted
			&& (!ft_strcmp(token->value, "|") || !ft_strcmp(token->value, "<")
				|| !ft_strcmp(token->value, ">") || !ft_strcmp(token->value,
					">>") || !ft_strcmp(token->value, "<<"))));
}

int	check_redirection_without_file(t_token *lst)
{
	t_token	*current;

	current = lst;
	while (current)
	{
		if (is_redirection_token(current))
		{
			if (!current->next)
			{
				ft_putstr_fd("minishell: syntax error", 2);
				ft_putstr_fd("near unexpected token `newline'\n", 2);
				return (1);
			}
			else if (current->next && is_invalid_next_token(current->next))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `",
					2);
				ft_putstr_fd(current->next->value, 2);
				ft_putstr_fd("'\n", 2);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}
