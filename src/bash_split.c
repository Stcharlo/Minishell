/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:17:39 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 12:04:58 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_special_char(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

static void	skip_whitespace(const char **s)
{
	while (isspace(**s))
		(*s)++;
}

static char	*extract_token(const char **s)
{
	t_token_info	info;

	info = extract_token_with_quote_info(s);
	return (info.value);
}

char	**split_bash_style(const char *input)
{
	const char	*p = input;
	size_t		alloc_count;
	char		**tokens;
	int			count;
	char		*token;

	alloc_count = strlen(input) / 2 + 5;
	tokens = malloc(sizeof(char *) * alloc_count);
	if (!tokens)
		return (NULL);
	count = 0;
	while (*p)
	{
		skip_whitespace(&p);
		if (*p == '\0')
			break ;
		token = extract_token(&p);
		if (token && token[0] != '\0')
		{
			tokens[count++] = token;
		}
	}
	tokens[count] = NULL;
	return (tokens);
}

t_token_info	*split_bash_style_with_quotes(const char *input,
		int *token_count)
{
	const char		*p = input;
	size_t			alloc_count;
	t_token_info	*tokens;
	int				count;
	t_token_info	info;

	alloc_count = strlen(input) / 2 + 5;
	tokens = malloc(sizeof(t_token_info) * alloc_count);
	if (!tokens)
		return (NULL);
	count = 0;
	while (*p)
	{
		skip_whitespace(&p);
		if (*p == '\0')
			break ;
		info = extract_token_with_quote_info(&p);
		if (info.value && info.value[0] != '\0')
		{
			tokens[count++] = info;
		}
	}
	*token_count = count;
	return (tokens);
}
