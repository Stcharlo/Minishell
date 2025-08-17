/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:10:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 12:55:14 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Check for pipe at the beginning of a command
 *
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
static int	check_pipe_at_start(t_token *lst)
{
	if (lst && (lst->type == PIPE || (!lst->was_quoted && !ft_strcmp(lst->value,
					"|"))))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

/**
 * Check for consecutive pipes
 *
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
static int	check_consecutive_pipes(t_token *lst)
{
	t_token	*current;

	current = lst;
	while (current && current->next)
	{
		if ((current->type == PIPE || (!current->was_quoted
					&& !ft_strcmp(current->value, "|")))
			&& (current->next->type == PIPE || (!current->next->was_quoted
					&& !ft_strcmp(current->next->value, "|"))))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
				2);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * Check for pipe at the end
 *
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
static int	check_pipe_at_end(t_token *lst)
{
	t_token	*current;

	current = lst;
	if (!current)
		return (0);
	while (current->next)
		current = current->next;
	if (current->type == PIPE || (!current->was_quoted
			&& !ft_strcmp(current->value, "|")))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
			2);
		return (1);
	}
	return (0);
}

/**
 * Check for invalid syntax in the token list
 *
 * @param lst The token list
 * @param env The AST environment
 * @return 1 if there's a syntax error, 0 otherwise
 */
int	check_syntax_errors(t_token *lst)
{
	if (check_pipe_at_start(lst) || check_consecutive_pipes(lst)
		|| check_redirection_without_file(lst) || check_pipe_at_end(lst)
		|| check_invalid_combinations(lst))
	{
		return (1);
	}
	return (0);
}
