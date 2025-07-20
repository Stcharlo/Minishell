/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:17:39 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/20 19:21:57 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_special_char(char c)
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
	const char *p = *s;
	char *token = malloc(strlen(p) + 3);
	if (!token)
		return NULL;

	int i = 0;
	int in_single_quote = 0, in_double_quote = 0;

	while (*p)
	{
		if (!in_single_quote && !in_double_quote)
		{
			if (isspace(*p))
				break;

			// Handle special characters
			if (is_special_char(*p))
			{
				if (i == 0)
				{
					token[i++] = *p++;

					// Check for >> or <<
					if ((*p == '>' || *p == '<') && token[0] == *p)
					{
						token[i++] = *p++;
					}
				}
				break;
			}
		}

		if (*p == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			p++;
			continue;
		}

		if (*p == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			p++;
			continue;
		}

		if (*p == '\\' && !in_single_quote)
		{
			p++;
			if (*p)
				token[i++] = *p++;
			continue;
		}

		token[i++] = *p++;
	}

	token[i] = '\0';
	*s = p;
	return token;
}

char	**split_bash_style(const char *input)
{
	const char *p = input;
	size_t alloc_count = strlen(input) / 2 + 5;
	char **tokens = malloc(sizeof(char *) * alloc_count);
	if (!tokens)
		return NULL;

	int count = 0;
	while (*p)
	{
		skip_whitespace(&p);
		if (*p == '\0')
			break;

		char *token = extract_token(&p);
		if (token && token[0] != '\0')
		{
			tokens[count++] = token;
		}
	}
	tokens[count] = NULL;
	return tokens;
}

void free_tokens(char **tokens)
{
	for (int i = 0; tokens[i]; i++)
		free(tokens[i]);
	free(tokens);
}
