/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:45:12 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 15:57:56 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_delimiter(char c, const char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (c == delim[i])
			return (1);
		i++;
	}
	return (0);
}

int	skip_spaces(const char *s, int i)
{
	while (s[i] && isspace((unsigned char)s[i]))
		i++;
	return (i);
}

int	segment_length(const char *s, const char *delim, int i)
{
	int	start;

	start = i;
	if (is_delimiter(s[i], delim))
	{
		while (s[i] && is_delimiter(s[i], delim))
			i++;
	}
	else
	{
		while (s[i] && !isspace((unsigned char)s[i])
			&& !is_delimiter(s[i], delim))
			i++;
	}
	return (i - start);
}

int	count_words(const char *s, const char *delim)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		i = skip_spaces(s, i);
		if (!s[i])
			break ;
		if (is_delimiter(s[i], delim))
		{
			while (s[i] && is_delimiter(s[i], delim))
				i++;
			count++;
		}
		else
		{
			while (s[i] && !isspace((unsigned char)s[i])
				&& !is_delimiter(s[i], delim))
				i++;
			count++;
		}
	}
	return (count);
}

void	free_all(char **psplit)
{
	int	word;

	word = 0;
	while (psplit[word])
	{
		free(psplit[word]);
		word++;
	}
	free(psplit);
}
