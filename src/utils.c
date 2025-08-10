/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:16:51 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 14:23:40 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_type(char *str)
{
	if (!strcmp(str, "|"))
		return (PIPE);
	else if (!strcmp(str, "<"))
		return (INPUT_REDIRECT);
	else if (!strcmp(str, "<<"))
		return (HEREDOC);
	else if (!strcmp(str, ">"))
		return (OUTPUT_REDIRECT);
	else if (!strcmp(str, ">>"))
		return (APPEND);
	else if (contains_meta_character(str))
		return (INVALID);
	else
		return (WORD);
}

ASTNode	*create_ast(int type, char *word)
{
	ASTNode	*node;

	node = malloc(sizeof(ASTNode));
	node->type = type;
	node->value = word;
	node->children = NULL;
	node->child_count = 0;
	return (node);
}

int	contains_meta_character(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_meta_character(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	is_meta_character(char c)
{
	int		i;
	char	*str;

	i = 0;
	str = " \t\n|&;()<>";
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	separate_tokens(char *str)
{
	int	i;
	int	count_letters;

	i = 0;
	count_letters = 0;
	while (str[i])
	{
		while (isspace(str[i]))
			i++;
		while (is_meta_character(str[i]))
		{
			count_letters++;
			i++;
		}
		while (str[i] && !isspace(str[i]) && !is_meta_character(str[i]))
		{
			count_letters++;
			i++;
		}
	}
}
