/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:54:19 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 13:03:14 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Helpers to respect 42 Norm for readability and line limits */
static int	is_redirection_token(t_token *tok)
{
	if (!tok)
		return (0);
	if (tok->type == INPUT_REDIRECT || tok->type == OUTPUT_REDIRECT
		|| tok->type == APPEND || tok->type == HEREDOC)
		return (1);
	if (!tok->was_quoted && tok->value && (!ft_strcmp(tok->value, "<")
			|| !ft_strcmp(tok->value, ">")
			|| !ft_strcmp(tok->value, ">>")
			|| !ft_strcmp(tok->value, "<<")))
		return (1);
	return (0);
}

static int	next_is_missing_or_invalid(t_token *tok)
{
	t_token	*next;

	if (!tok)
		return (1);
	next = tok->next;
	if (!next || next->type != WORD)
		return (1);
	if (next->was_quoted == 0 && (next->type == PIPE
			|| next->type == INPUT_REDIRECT || next->type == OUTPUT_REDIRECT
			|| next->type == APPEND || next->type == HEREDOC))
		return (1);
	return (0);
}

static void	print_syntax_error_token(t_token *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (tok)
		ft_putstr_fd(tok->value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
}

/**
 * Check for redirection operators without proper file arguments
 *
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
int	check_redirection_without_file(t_token *lst)
{
	t_token	*current;

	current = lst;
	while (current)
	{
		if (is_redirection_token(current)
			&& next_is_missing_or_invalid(current))
		{
			print_syntax_error_token(current->next);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * Check for invalid token combinations not covered by other checks
 *
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
int	check_invalid_combinations(t_token *lst)
{
	t_token	*current;
	t_token	*next;

	current = lst;
	while (current)
	{
		next = current->next;
		if ((current->type == PIPE || (!current->was_quoted
					&& !ft_strcmp(current->value, "|"))) && next
			&& (next->type == INPUT_REDIRECT || next->type == OUTPUT_REDIRECT
				|| next->type == APPEND || next->type == HEREDOC)
			&& (!next->next || next->next->type != WORD))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(next->value, 2);
			ft_putstr_fd("'\n", 2);
			return (1);
		}
		current = current->next;
	}
	return (0);
}
