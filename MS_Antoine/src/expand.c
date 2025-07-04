/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:10:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/04 16:05:06 by stcharlo         ###   ########.fr       */
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

    i = 0;
    while (str[i])
    {
        if (str[i] == '\'')
        {
            i++;
            while (str[i] && str[i] != '\'')
                i++;
            if (str[i])
                i++;
        }
        else if (str[i] == '$')
        {
            start = i;
            len = 1;
            while (str[i + len] && ((str[i + len] >= 'A' && str[i + len] <= 'Z')
                || (str[i + len] >= 'a' && str[i + len] <= 'z')
                || (str[i + len] == '_')))
                len++;
            str = expand_one(str, start, len, env);
            i = 0;
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
static char	*expand_one(const char *str, int start, int len, t_ast **env)
{
    char	**cmd;
    char	*var;
    char	*val;
    char	*res;

    cmd = ft_split_dollar_range(str, start, start + len);
    var = ft_substr(str, start + 1, len - 1);
    val = get_value(var, len - 1, env);
    if (!val)
        val = "";
    res = ft_strjoin(cmd[0], val);
    printf("---expand_one---\nstart: %d\nlen: %d\ncmd[0]: %s\ncmd[1]: %s\ncmd[2]: %s\n,var: %s\nval: %s\n", start, len, cmd[0], cmd[1], cmd[2], var, val);
    printf("expand_one res: %s\n", res);
    if (cmd[2])
        res = ft_strjoin(res, cmd[2]);
    printf("expand_one res: %s\n", res);
    free(var);
    free(cmd[0]);
    if (cmd[1])
        free(cmd[1]);
    if (cmd[2])
        free(cmd[2]);
    free(cmd);
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
static int	find_next_expand(const char *str, int *start, int *len)
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
