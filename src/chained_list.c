/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chained_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:24:00 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/11 18:01:52 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_type_v2(char *str, char *str_index)
{
	if (str_index[0] == 'a')
		return (check_type(str));
	else
		return (WORD);
}

t_token	*ft_lstnew(char *str, char *str_index)
{
	t_token	*element;

	element = malloc(sizeof(t_token));
	if (!element)
		return (0);
	element->value = str;
	element->type = check_type_v2(str, str_index);
	element->next = NULL;
	element->prev = NULL;
	return (element);
}

int	create_list(t_token **start, char **str, char **str_index)
{
	t_token		*new;
	int i;

	i = 0;
	while (str[i])
	{
		new = ft_lstnew(str[i], str_index[i]);
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
void ft_lstadd_back(t_token **lst, t_token *new, char *str)
{
	int i;
	t_token *current;

	i = ft_lstsize(*lst);
	new->value = str;
	// new->type = check_type_v2(str);
	new->next = NULL;
	new->prev = NULL;
	if (!*lst)
	{
		*lst = new;
		return;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current; // <-- Set prev pointer
}
int ft_lstsize(t_token *lst)
{
	int i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
void free_stack(t_token **stack)
{
	t_token *current;
	t_token *next;

	if (!*stack)
		return;
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

void show_list(t_token *list)
{
	while (list)
	{
		printf("Token: %s, Type: %u\n", list->value, list->type);
		list = list->next;
	}
	printf("NULL\n");
}
