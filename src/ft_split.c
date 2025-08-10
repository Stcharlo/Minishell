/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:41:54 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 16:05:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

typedef struct s_segment
{
	const char	*s;
	int			start;
	int			len;
	int			word;
}	t_segment;

static int	copy_segment(char **psplit, t_segment seg)
{
	psplit[seg.word] = (char *)malloc(sizeof(char) * (seg.len + 1));
	if (!psplit[seg.word])
		return (-1);
	ft_strlcpy(psplit[seg.word], seg.s + seg.start, seg.len + 1);
	return (0);
}

static void	setup_segment(t_segment *seg, const char *s, int word)
{
	seg->s = s;
	seg->word = word;
}

static int	process_next_word(char **psplit, const char *s,
			const char *delim, int *word)
{
	int			i;
	int			start;
	int			len;
	t_segment	seg;

	i = skip_spaces(s, 0);
	if (!s[i])
		return (0);
	start = i;
	len = segment_length(s, delim, i);
	setup_segment(&seg, s, *word);
	seg.start = start;
	seg.len = len;
	if (copy_segment(psplit, seg) == -1)
		return (-1);
	(*word)++;
	return (i + len);
}

static int	fill_split(char **psplit, const char *s, const char *delim)
{
	int	i;
	int	word;
	int	offset;

	i = 0;
	word = 0;
	while (s[i])
	{
		offset = process_next_word(psplit, s + i, delim, &word);
		if (offset == -1)
		{
			free_all(psplit);
			return (-1);
		}
		if (offset == 0)
			break ;
		i += offset;
	}
	psplit[word] = NULL;
	return (0);
}

char	**ft_split(char *s, const char *delim)
{
	char	**psplit;
	int		count;

	if (!s || !delim)
		return (NULL);
	count = count_words(s, delim);
	psplit = (char **)malloc(sizeof(char *) * (count + 1));
	if (!psplit)
		return (NULL);
	if (fill_split(psplit, s, delim) == -1)
		return (NULL);
	return (psplit);
}
