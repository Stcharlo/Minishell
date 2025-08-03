/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:10:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/03 06:58:34 by agaroux          ###   ########.fr       */
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
	if (lst && (lst->type == PIPE || (!lst->was_quoted && !ft_strcmp(lst->value, "|"))))
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
		if ((current->type == PIPE || (!current->was_quoted && !ft_strcmp(current->value, "|"))) && 
			(current->next->type == PIPE || (!current->next->was_quoted && !ft_strcmp(current->next->value, "|"))))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * Check for redirection without a filename
 * 
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
static int	check_redirection_without_file(t_token *lst)
{
	t_token	*current;

	current = lst;
	while (current)
	{
		// Check all types of redirections (only if not quoted)
		if ((current->type == INPUT_REDIRECT || current->type == OUTPUT_REDIRECT || 
			current->type == APPEND || current->type == HEREDOC) || 
			(!current->was_quoted && (!ft_strcmp(current->value, "<") || !ft_strcmp(current->value, ">") || 
			!ft_strcmp(current->value, ">>") || !ft_strcmp(current->value, "<<"))))
		{
			// No argument after redirection
			if (!current->next)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (1);
			}
			// Another operator after redirection
			else if (current->next && 
				(current->next->type == PIPE || current->next->type == INPUT_REDIRECT || 
				current->next->type == OUTPUT_REDIRECT || current->next->type == APPEND || 
				current->next->type == HEREDOC ||
				(!current->next->was_quoted && (!ft_strcmp(current->next->value, "|") || !ft_strcmp(current->next->value, "<") || 
				!ft_strcmp(current->next->value, ">") || !ft_strcmp(current->next->value, ">>") || 
				!ft_strcmp(current->next->value, "<<")))))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				ft_putstr_fd(current->next->value, 2);
				ft_putstr_fd("'\n", 2);
				return (1);
			}
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
		
	if (current->type == PIPE || (!current->was_quoted && !ft_strcmp(current->value, "|")))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		return (1);
	}
	return (0);
}

/**
 * Check for invalid token combinations not covered by other checks
 * 
 * @param lst The token list
 * @return 1 if there's a syntax error, 0 otherwise
 */
static int	check_invalid_combinations(t_token *lst)
{
	t_token	*current;
	t_token	*next;

	current = lst;
	while (current)
	{
		next = current->next;
		
		// Example: checking for cases like "echo hi | >" or other uncovered syntax errors
		if ((current->type == PIPE || (!current->was_quoted && !ft_strcmp(current->value, "|"))) && next && 
			(next->type == INPUT_REDIRECT || next->type == OUTPUT_REDIRECT || 
			next->type == APPEND || next->type == HEREDOC) && 
			(!next->next || next->next->type != WORD))
		{
			// This catches cases where a pipe is followed by a redirection without a command
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(next->value, 2);
			ft_putstr_fd("'\n", 2);
			return (1);
		}
		
		current = current->next;
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
	// Check all syntax errors
	if (check_pipe_at_start(lst) || 
		check_consecutive_pipes(lst) ||
		check_redirection_without_file(lst) ||
		check_pipe_at_end(lst) ||
		check_invalid_combinations(lst))
	{
		// The error code will be set by the calling function
		return (1);
	}
	return (0);
}
