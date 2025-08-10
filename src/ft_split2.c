/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:34:30 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 15:44:26 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	find_next_dollar(const char *s, int start, int end)
{
	int	i;

	i = start;
	while (s[i] && i <= end)
	{
		if (s[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

static int	is_var_char(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	if (c == '_' || c == '?')
		return (1);
	return (0);
}

static int	var_end_index(const char *s, int i, int end)
{
	int	j;

	j = i + 1;
	while (s[j] && j <= end && is_var_char(s[j]))
	{
		if (s[j] == '?')
		{
			j++;
			break ;
		}
		j++;
	}
	return (j);
}

static void	set_tail(char **arr, const char *s, int j)
{
	if (s[j])
		arr[2] = ft_strdup(s + j);
	else
		arr[2] = NULL;
}

char	**ft_split_dollar_range(const char *s, int start, int end)
{
	char	**arr;
	int		i;
	int		j;

	if (!s || start < 0 || end < start)
		return (NULL);
	i = find_next_dollar(s, start, end);
	arr = (char **)malloc(sizeof(char *) * 4);
	if (!arr)
		return (NULL);
	if (i != -1)
	{
		arr[0] = ft_substr(s, 0, i);
		j = var_end_index(s, i, end);
		arr[1] = ft_substr(s, i, j - i);
		set_tail(arr, s, j);
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
