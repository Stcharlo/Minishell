/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:59:03 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 13:03:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// @brief eliminating signle quotes
/// @param str
/// @param start start of single quotes
/// @param end end of single quotes
/// @return
char	*single_quotes(char *str, int start, int end)
{
	char	*res;
	int		index_str;
	int		index_res;

	index_str = 0;
	index_res = 0;
	res = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (index_str < start)
		res[index_res++] = str[index_str++];
	while (index_str <= end)
	{
		if (str[index_str] != '\'')
			res[index_res++] = str[index_str++];
		else
			index_str++;
	}
	while (str[index_str])
		res[index_res++] = str[index_str++];
	res[index_res] = 0;
	free(str);
	return (res);
}

/// @brief check that quotes are still open while reading the line
/// @param str
/// @return true if one quote is still open
int	open_quotes(const char *str)
{
	int	i;
	int	s;
	int	d;

	i = 0;
	s = 0;
	d = 0;
	while (str[i])
	{
		if (str[i] == '\'' && d % 2 == 0)
			s++;
		else if (str[i] == '"' && s % 2 == 0)
			d++;
		i++;
	}
	return (s % 2 != 0 || d % 2 != 0);
}

/// @brief reading user input while quotes are still open
/// @param str
/// @return
char	*readline_open_quotes(char *str)
{
	char	*tmp;
	char	*res;

	tmp = readline("quote> ");
	if (!tmp)
	{
		free(str);
		return (NULL);
	}
	res = ft_strjoin_buffer(str, tmp, '\n');
	free(str);
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}
