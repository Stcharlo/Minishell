/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_var_expansion.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:16:10 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 11:41:50 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_quote(char c, int *in_quotes, int *i)
{
	*in_quotes = !(*in_quotes);
	(*i)++;
}

static int	is_valid_var_char(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	if (c == '_' || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

static int	get_var_len(char *str, int i)
{
	int	len;

	len = 1;
	if (str[i + 1] == '?')
		return (2);
	while (str[i + len] && is_valid_var_char(str[i + len]))
		len++;
	return (len);
}

static char	*handle_var_expansion(char *str, int start, t_ast **env, int *i)
{
	int	len;

	len = get_var_len(str, start);
	if ((len > 1) || (str[start + 1] == '?'))
	{
		str = expand_one(str, start, len, env);
		*i = 0;
	}
	else
		(*i)++;
	return (str);
}

/// @brief reading user input and checking
/// for variables that need to get expanded
/// @param str user input
/// @param env
/// @return new line with expanded variables
char	*unquoted_var_expansion(char *str, t_ast **env)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			handle_quote(str[i], &in_single_quotes, &i);
		else if (str[i] == '"' && !in_single_quotes)
			handle_quote(str[i], &in_double_quotes, &i);
		else if (str[i] == '$' && !in_single_quotes)
			str = handle_var_expansion(str, i, env, &i);
		else
			i++;
	}
	return (str);
}
