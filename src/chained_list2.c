/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chained_list2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:24:00 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/20 15:55:42 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_back_with_quote_info(t_token **lst, t_token *new, char *str,
		int was_quoted)
{
	t_token	*current;

	new->value = str;
	new->was_quoted = was_quoted;
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
