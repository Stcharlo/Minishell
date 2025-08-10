/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:08:17 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 11:41:17 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_single_quote(const char *str, int i, int *in_single_quotes,
		int in_double_quotes)
{
	if (str[i] == '\'' && !in_double_quotes)
	{
		*in_single_quotes = !(*in_single_quotes);
		return (1);
	}
	return (0);
}

static int	handle_double_quote(const char *str, int i, int in_single_quotes,
		int *in_double_quotes)
{
	if (str[i] == '"' && !in_single_quotes)
	{
		*in_double_quotes = !(*in_double_quotes);
		return (1);
	}
	return (0);
}

static int	handle_dollar(char **str, int *i, t_ast **env)
{
	int	start;
	int	len;

	start = *i;
	len = 1;
	if ((*str)[*i + 1] == '?')
	{
		len = 2;
		*str = expand_one(*str, start, len, env);
		*i = 0;
		return (1);
	}
	while ((*str)[*i + len] && is_var_char((*str)[*i + len]))
		len++;
	if (len > 1)
	{
		*str = expand_one(*str, start, len, env);
		*i = 0;
		return (1);
	}
	return (0);
}

void	handle_quotes(const char *str, int i,
	int *in_single_quotes, int *in_double_quotes)
{
	if (handle_single_quote(str, i, in_single_quotes, *in_double_quotes))
		return ;
	if (handle_double_quote(str, i, *in_single_quotes, in_double_quotes))
		return ;
}

int	handle_var_expansion(char **str, int *i, t_ast **env,
	int in_single_quotes)
{
	if ((*str)[*i] == '$' && !in_single_quotes)
	{
		if (handle_dollar(str, i, env))
			return (1);
		(*i)++;
		return (1);
	}
	return (0);
}
