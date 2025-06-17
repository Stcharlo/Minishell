/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:10:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/17 17:59:11 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

char	*unquoted_var_expansion(char *str, char **env)
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
            str = expand_unquoted_var_at(str, start, len, env);
            i = 0;
        }
        else
            i++;
    }
    return (str);
}
//This function may be useless, need further testing
char	*expand_unquoted_var_at(char *str, int start, int len, char **env)
{
    char	*expanded;
    char	*prefix;
    char	*suffix;
    char	*tmp;

    expanded = expand_one(str, start, len, env);
    prefix = ft_substr(str, 0, start);
    suffix = ft_strdup(str + start + len);
    printf("---expand_unquoted_var_at---\nExpanded: %s\nPrefix: %s\nSuffix: %s\n", expanded, prefix, suffix);
    //tmp = ft_strjoin(prefix, expanded);
    //free(prefix);
    //free(expanded);
    //free(str);
    //expanded = ft_strjoin(tmp, suffix);
    //free(tmp);
    //free(suffix);
    printf("expand_unquoted_var_at Expanded: %s\n", expanded);
    return (expanded);
}
//key function to expand outside of quote
static char	*expand_one(const char *str, int start, int len, char **env)
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

char	*expand_variable(char *str, char **env)
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