/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:41:54 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/13 14:07:27 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int is_delim(char c, const char *delims) {
    while (*delims)
        if (c == *delims++)
            return 1;
    return 0;
}

char **split_quote_aware(const char *s, const char *delims) {
    char **result = malloc(sizeof(char*) * (strlen(s) + 2));
    int i = 0, j = 0, k = 0, in_single = 0, in_double = 0;
    char buf[4096];

    while (s[i]) {
        while (isspace(s[i]))
            i++;
        if (!s[i]) break;
        k = 0;
        in_single = in_double = 0;
        while (s[i]) {
            if (s[i] == '\'' && !in_double) in_single = !in_single, i++;
            else if (s[i] == '"' && !in_single) in_double = !in_double, i++;
            else if (!in_single && !in_double && is_delim(s[i], delims)) break;
            else buf[k++] = s[i++];
        }
        buf[k] = 0;
        result[j++] = strdup(buf);
        if (s[i] && is_delim(s[i], delims)) i++;
    }
    result[j] = NULL;
    return result;
}

char	**ft_split(char *s, const char *delim)
{
	char	**psplit;
	int		count;

	if (!s || !delim)
		return (NULL);
	count = count_words(s, delim);
	psplit = malloc(sizeof(char *) * (count + 1));
	if (!psplit)
		return (NULL);
	if (split_word(psplit, s, delim, 0) == -1)
		return (NULL);
    
	return (psplit);
}

char	**ft_split_index(char *s, const char *delim, char *s_index)
{
	char	**psplit;
	int		count;

	if (!s || !delim)
		return (NULL);
	count = count_words(s, delim);
	psplit = malloc(sizeof(char *) * (count + 1));
	if (!psplit)
		return (NULL);
	if (split_word_index(psplit, s, delim, s_index) == -1)
		return (NULL);
    
	return (psplit);
}

int	split_word_index(char **psplit, const char *s, const char *delim,
		char *s_index)
{
    int word = 0;
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
        ft_strlcpy(psplit[word], s_index + start, len + 1);
        word++;
    }
    psplit[word] = NULL;
    return (1);
}

char	**ft_split_once_range(const char *s, char sep, int start, int end)
{
    char	**res;
    int		i;

    if (!s || start < 0 || end < start)
        return (NULL);
    i = start;
    while (s[i] && i <= end && s[i] != sep)
        i++;
    res = malloc(sizeof(char *) * 3);
    if (!res)
        return (NULL);
    if (s[i] == sep && i <= end)
    {
        res[0] = ft_substr(s, 0, i);
        res[1] = ft_substr(s, i + 1, ft_strlen(s) - i - 1);
    }
    else
    {
        res[0] = ft_strdup(s);
        res[1] = NULL;
    }
    res[2] = NULL;
    return (res);
}

char	**ft_split_dollar_range(const char *s, int start, int end)
{
    char	**arr;
    int		i, j;

    if (!s || start < 0 || end < start)
        return (NULL);
    i = start;
    while (s[i] && i <= end && s[i] != '$')
        i++;
    arr = malloc(sizeof(char *) * 4);
    if (!arr)
        return (NULL);
    if (s[i] == '$' && i <= end)
    {
        arr[0] = ft_substr(s, 0, i);
        j = i + 1;
        while (s[j] && ((s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= 'a' && s[j] <= 'z') || (s[j] == '_') || (s[j] == '?')))
            j++;
        arr[1] = ft_substr(s, i, j - i);
        arr[2] = s[j] ? ft_strdup(s + j) : NULL;
    }
    else
    {
        arr[0] = ft_strdup(s);
        arr[1] = NULL;
        arr[2] = NULL;
    }
    arr[3] = NULL;
    return (arr);
}
