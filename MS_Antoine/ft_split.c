/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:41:54 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/12 17:58:41 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

static int	is_delimiter(char c, const char *delim)
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

static int	count_words(const char *s, const char *delim)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	
	while (s[i])
	{
		while (isspace(s[i]))
			i++;
		if (!s[i])
			break;
		if (is_delimiter(s[i], delim))
		{
			count++;
			i++;
		}
		else
		{
			while (s[i] && !isspace(s[i]) && !is_delimiter(s[i], delim))
				i++;
			count++;
		}
	}
	return (count);
}

static void	free_all(char **psplit)
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

static int	split_word(char **psplit, const char *s, const char *delim,
		int word)
{
    int i = 0;
    int start;

    while (s[i])
    {
        while (s[i] && isspace(s[i]))
            i++;
        if (!s[i])
            break;

        start = i;
        if (is_delimiter(s[i], delim))
			{
		while (is_delimiter(s[i], delim))
            i++;
        }
        else
        {
            while (s[i] && !isspace(s[i]) && !is_delimiter(s[i], delim))
                i++;
        }
        int len = i - start;
        psplit[word] = malloc(sizeof(char) * (len + 1));
        if (!psplit[word])
        {
            free_all(psplit);
            return (-1);
        }
        ft_strlcpy(psplit[word], s + start, len + 1);
        word++;
    }
    psplit[word] = NULL;
    return (1);
}

char	**ft_split(const char *s, const char *delim)
{
	char	**psplit;
	int		count;

	if (!s || !delim)
		return (NULL);
	count = count_words(s, delim);
	printf("count: %d\n", count);
	psplit = malloc(sizeof(char *) * (count + 1));
	if (!psplit)
		return (NULL);
	if (split_word(psplit, s, delim, 0) == -1)
		return (NULL);
	return (psplit);
}
