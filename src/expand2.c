/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:30:19 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 11:43:58 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// @brief Safe version of ft_substr with empty string fallback
/// @param str Source string
/// @param start Starting position
/// @param len Length to extract
/// @return Substring or empty string if allocation fails
static char	*safe_substr(const char *str, int start, int len)
{
	char	*result;

	result = ft_substr(str, start, len);
	if (!result)
		result = ft_strdup("");
	return (result);
}

/// @brief Extract variable parts (before, var name, after)
/// @param parts Array to store the parts
/// @param str Original string
/// @param start Start position of variable
/// @param len Length of variable reference
static void	extract_var_parts(char **parts, const char *str, int start, int len)
{
	parts[0] = safe_substr(str, 0, start);
	parts[1] = safe_substr(str, start + 1, len - 1);
	parts[2] = ft_strdup(str + start + len);
	if (!parts[2])
		parts[2] = ft_strdup("");
}

/// @brief Join parts and build final result
/// @param parts Array of string parts
/// @param val Expanded variable value
/// @return Joined string with variable expanded
static char	*join_expanded_parts(char **parts, char *val)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(parts[0], val);
	if (!tmp)
		tmp = ft_strdup("");
	res = ft_strjoin(tmp, parts[2]);
	if (!res)
		res = ft_strdup("");
	free(tmp);
	return (res);
}

/// @brief main function to expand outside of quote
/// @param str Input string
/// @param start Position of $ in string
/// @param len Length of variable reference (including $)
/// @param env Environment data
/// @return New string with variable expanded
char	*expand_one(const char *str, int start, int len, t_ast **env)
{
	char	*parts[3];
	char	*val;
	char	*res;

	if (!str || start < 0 || len <= 1)
	{
		if (str)
			return (ft_strdup(str));
		else
			return (ft_strdup(""));
	}
	extract_var_parts(parts, str, start, len);
	val = get_value(parts[1], len - 1, env);
	if (!val)
		val = ft_strdup("");
	res = join_expanded_parts(parts, val);
	free(parts[0]);
	free(parts[1]);
	free(parts[2]);
	free(val);
	return (res);
}
