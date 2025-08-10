/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:02:32 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 13:02:53 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	find_quote_end(const char *str, int i, char quote)
{
	while (str[i] && str[i] != quote)
		i++;
	return (i);
}

static char	*handle_single_quotes(char *str, int *i)
{
	int	start;
	int	end;

	start = *i;
	end = find_quote_end(str, ++(*i), '\'');
	if (str[end] == '\'')
	{
		str = single_quotes(str, start, end);
		*i = start - 1;
	}
	else
		*i = end;
	return (str);
}

static char	*handle_double_quotes(char *str, int *i, t_ast **env)
{
	int	start;
	int	end;

	start = *i;
	end = find_quote_end(str, ++(*i), '\"');
	if (str[end] == '\"')
	{
		str = double_quotes(str, start, end, env);
		*i = start - 1;
	}
	else
		*i = end;
	return (str);
}

/// @brief formatting the lines if their are single of double quotes
/// @param str
/// @param env
/// @return
char	*clean_line(char *str, t_ast **env)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			str = handle_single_quotes(str, &i);
		else if (str[i] == '\"')
			str = handle_double_quotes(str, &i, env);
		i++;
	}
	return (str);
}
