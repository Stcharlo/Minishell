/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chained_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:24:00 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/03 14:51:59 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*ft_lstnew(char *str)
{
	t_token	*element;

	element = malloc(sizeof(t_token));
	if (!element)
		return (0);
	element->value = str;
	element->type = check_type(str);
	element->was_quoted = 0;
	element->next = NULL;
	element->prev = NULL;
	return (element);
}

t_token	*ft_lstnew_with_quote_info(char *str, int was_quoted)
{
	t_token	*element;

	element = malloc(sizeof(t_token));
	if (!element)
		return (0);
	element->value = str;
	element->was_quoted = was_quoted;
	// Only assign operator types if the token was not quoted
	if (was_quoted)
		element->type = WORD;
	else
		element->type = check_type(str);
	element->next = NULL;
	element->prev = NULL;
	return (element);
}

int	create_list(t_token **start, char **str)
{
	t_token	*new;
	int		i;

	i = 0;
	while (str[i])
	{
		new = ft_lstnew(str[i]);
		if (!new)
		{
			free_stack(start);
			return (0);
		}
		ft_lstadd_back(start, new, str[i]);
		i++;
	}
	return (1);
}

int	create_list_with_quote_info(t_token **start, t_token_info *tokens, int token_count)
{
	t_token	*new;
	int		i;

	i = 0;
	while (i < token_count)
	{
		new = ft_lstnew_with_quote_info(tokens[i].value, tokens[i].was_quoted);
		if (!new)
		{
			free_stack(start);
			return (0);
		}
		ft_lstadd_back_with_quote_info(start, new, tokens[i].value, tokens[i].was_quoted);
		i++;
	}
	return (1);
}

void	ft_lstadd_back(t_token **lst, t_token *new, char *str)
{
	t_token	*current;

	new->value = str;
	new->type = check_type(str);
	new->was_quoted = 0;
	new->next = NULL;
	new->prev = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}

void	ft_lstadd_back_with_quote_info(t_token **lst, t_token *new, char *str, int was_quoted)
{
	t_token	*current;

	new->value = str;
	new->was_quoted = was_quoted;
	// Only assign operator types if the token was not quoted
	if (was_quoted)
		new->type = WORD;
	else
		new->type = check_type(str);
	new->next = NULL;
	new->prev = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}

int	ft_lstsize(t_token *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	free_stack(t_token **stack)
{
	t_token	*current;
	t_token	*next;

	if (!*stack)
		return ;
	current = *stack;
	next = current->next;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*stack = NULL;
}

