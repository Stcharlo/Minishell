/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:17:39 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/03 06:58:34 by agaroux          ###   ########.fr       */
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

static t_token_info	extract_token_with_quote_info(const char **s)
{
	const char *p = *s;
	char *token = malloc(strlen(p) + 3);
	t_token_info info = {NULL, 0};
	
	if (!token)
		return info;

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
			info.was_quoted = 1;
			p++;
			continue;
		}

		if (*p == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			info.was_quoted = 1;
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
	info.value = token;
	return info;
}

static char	*extract_token(const char **s)
{
	t_token_info info = extract_token_with_quote_info(s);
	return info.value;
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

t_token_info	*split_bash_style_with_quotes(const char *input, int *token_count)
{
	const char *p = input;
	size_t alloc_count = strlen(input) / 2 + 5;
	t_token_info *tokens = malloc(sizeof(t_token_info) * alloc_count);
	if (!tokens)
		return NULL;

	int count = 0;
	while (*p)
	{
		skip_whitespace(&p);
		if (*p == '\0')
			break;

		t_token_info info = extract_token_with_quote_info(&p);
		if (info.value && info.value[0] != '\0')
		{
			tokens[count++] = info;
		}
	}
	*token_count = count;
	return tokens;
}

void free_tokens(char **tokens)
{
	for (int i = 0; tokens[i]; i++)
		free(tokens[i]);
	free(tokens);
}
