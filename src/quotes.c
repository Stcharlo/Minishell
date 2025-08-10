/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:28:27 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 13:03:09 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_exit_code;

/// @brief checking for the variable in the environment
/// @param var
/// @param n
/// @param env
/// @return value of variable
static char	*get_exit_code_str(t_ast **env)
{
	int	exit_code;

	if (g_exit_code >= 128)
		exit_code = g_exit_code;
	else
		exit_code = (*env)->env->error_code;
	return (ft_itoa(exit_code % 256));
}

static char	*get_env_value(char *var, int n, t_ast **env)
{
	t_ast	*current;
	int		i;

	current = *env;
	i = 0;
	while (current->env->env[i])
	{
		if (!strncmp(current->env->env[i], var, n)
			&& current->env->env[i][n] == '=')
			return (ft_strdup(current->env->env[i] + n + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*get_value(char *var, int n, t_ast **env)
{
	if (!strcmp(var, "?"))
		return (get_exit_code_str(env));
	return (get_env_value(var, n, env));
}

/// @brief eliminating double quotes & expanding variables
/// @param str
/// @param start
/// @param end
/// @param env
/// @return new clean string
char	*double_quotes(char *str, int start, int end, t_ast **env)
{
	char	*res;
	int		index_str;
	int		index_res;

	index_str = 0;
	index_res = 0;
	str = expand_variable(str, env);
	end = start + 1;
	while (str[end] && str[end] != '\"')
		end++;
	res = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (index_str < start)
		res[index_res++] = str[index_str++];
	while (index_str <= end)
	{
		if (str[index_str] != '\"')
			res[index_res++] = str[index_str++];
		else
			index_str++;
	}
	while (str[index_str])
		res[index_res++] = str[index_str++];
	return (res[index_res] = 0, free(str), res);
}
