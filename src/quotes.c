/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:28:27 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/13 13:14:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int g_exit_code;

/// @brief checking for the variable in the environment
/// @param var 
/// @param n 
/// @param env 
/// @return value of variable
char	*get_value(char *var, int n, t_ast **env)
{
	int	i;
	t_ast *current;

	current = *env;
	i = 0;
	if (!strcmp(var, "?"))
		return (ft_itoa(g_exit_code));
	while (current->env->env[i])
	{
		if (!strncmp(current->env->env[i], var, n) && current->env->env[i][n] == '=')
			return (current->env->env[i] + n + 1);
		i++;
	}
	if (!current->env->env[i])
		return (NULL);
	return (NULL);
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

/// @brief formatting the lines if their are single of double quotes
/// @param str
/// @param env
/// @return
char	*clean_line(char *str, t_ast **env)
{
	int	i;
	int	start;
	int	end;

	start = 0;
	end = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			start = i++;
			while (str[i] && str[i] != '\'')
				i++;
			end = i;
			if (str[i--] == '\'')
			{
				str = single_quotes(str, start, end);
				i--;
			}
		}
		else if (str[i] == '\"')
		{
			start = i++;
			while (str[i] && str[i] != '\"')
				i++;
			end = i;
			if (str[i--] == '\"')
			{
				str = double_quotes(str, start, end, env);
				i--;
			}
		}
		i++;
	}
	return (str);
}

/// @brief check that quotes are still open while reading the line
/// @param str
/// @return true if one quote is still open
int	open_quotes(const char *str)
{
	int	s = 0, d = 0, i;

	s = 0, d = 0, i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && d % 2 == 0)
			s++;
		else if (str[i] == '\"' && s % 2 == 0)
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