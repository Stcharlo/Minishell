/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:10:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/02 13:33:12 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// @brief reading user input and checking for variables that need to get expanded
/// @param str user input
/// @param env 
/// @return new line with expanded variables
char	*unquoted_var_expansion(char *str, t_ast **env)
{
    int	i;
    int	start;
    int	len;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    if (!str)
        return (NULL);

    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            i++;
        }
        else if (str[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            i++;
        }
        else if (str[i] == '$' && !in_single_quotes)
        {
            start = i;
            len = 1;
            
            // Handle $? specifically
            if (str[i + 1] == '?')
            {
                len = 2;
                str = expand_one(str, start, len, env);
                i = 0; // Restart from beginning after expansion
                continue;
            }
            
            // Handle other variables
            while (str[i + len] && ((str[i + len] >= 'A' && str[i + len] <= 'Z')
                || (str[i + len] >= 'a' && str[i + len] <= 'z')
                || (str[i + len] == '_')
                || (str[i + len] >= '0' && str[i + len] <= '9')))
                len++;
                
            if (len > 1)
            {
                str = expand_one(str, start, len, env);
                i = 0; // Restart from beginning after expansion
                continue;
            }
            else
            {
                // Single $ with no valid variable name - leave as is
                i++;
            }
        }
        else
            i++;
    }
    return (str);
}

/// @brief main function to expand outside of quote
/// @param str 
/// @param start 
/// @param len 
/// @param env 
/// @return 
char	*expand_one(const char *str, int start, int len, t_ast **env)
{
    char	*var;
    char	*val;
    char	*before;
    char	*after;
    char	*res;
    char	*tmp;

    if (!str || start < 0 || len <= 1)
        return (ft_strdup(str ? str : ""));

    // Get the part before the variable
    before = ft_substr(str, 0, start);
    if (!before)
        before = ft_strdup("");

    // Get the variable name (skip the $)
    var = ft_substr(str, start + 1, len - 1);
    if (!var)
        var = ft_strdup("");

    // Get the variable value
    val = get_value(var, len - 1, env);
    if (!val)
        val = ft_strdup("");

    // Get the part after the variable
    after = ft_strdup(str + start + len);
    if (!after)
        after = ft_strdup("");

    // Combine all parts
    tmp = ft_strjoin(before, val);
    if (!tmp)
        tmp = ft_strdup("");
    
    res = ft_strjoin(tmp, after);
    if (!res)
        res = ft_strdup("");

    // Clean up
    free(var);
    free(before);
    free(after);
    free(val);
    free(tmp);

    return (res);
}

/// @brief expanding variables
/// @param str 
/// @param env 
/// @return 
char	*expand_variable(char *str, t_ast **env)
{
    int		start;
    int		len;
    char	*res;

    if (find_next_expand(str, &start, &len))
    {
        res = expand_one(str, start, len, env);
        free(str);
        if (ft_strchr(res, '$'))
            return (expand_variable(res, env));
        return (res);
    }
    return (str);
}
/// @brief finding next varibale to expand
/// @param str 
/// @param start 
/// @param len 
/// @return 
int	find_next_expand(const char *str, int *start, int *len)
{
    int	i;
    int	j;
    int	sq;
    int	dq;

    i = 0;
    sq = 0;
    dq = 0;
    while (str[i])
    {
        if (str[i] == '\'' && dq % 2 == 0)
            sq++;
        else if (str[i] == '\"' && sq % 2 == 0)
            dq++;
        if (str[i] == '$' && dq % 2 == 1 && sq % 2 == 0)
        {
            j = 1;
            while (str[i + j] && ((str[i + j] >= 'A' && str[i + j] <= 'Z')
                || (str[i + j] == '_')))
                j++;
            *start = i;
            *len = j;
            return (1);
        }
        i++;
    }
    return (0);
}
