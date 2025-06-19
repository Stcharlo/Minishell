/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:46:39 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/19 16:15:57 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_value(char *var, int n, char **env)
{
    int i;
    
    i = 0;
    while (env[i])
    {
        if (!strncmp(env[i], var, n) && env[i][n] == '=')
            return (printf("return get_value: %s\n", env[i] + n + 1), env[i] + n + 1);
        i++;
    }
    if (!env[i])
        return (NULL);
    return (NULL);
}

//eliminating double quotes & expanding variables
char *double_quotes(char *str, int start, int end, char **env)
{
    char *res;
    int index_str;
    int index_res;

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
    res[index_res] = 0;
    return (free(str), res);
}
//eliminating single_quotes
char *single_quotes(char *str, int start, int end)
{
    char *res;
    int index_str;
    int index_res;

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

//formatting the lines if their are single of double quotes
char *clean_line(char *str, char **env)
{
    int i;
    int start;
    int end;

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

// check that quotes are still open while reading the line
int open_quotes(const char *str)
{
    int s = 0, d = 0, i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && d % 2 == 0)
            s++;
        else if (str[i] == '\"' && s % 2 == 0)
            d++;
        i++;
    }
    printf("s: %d, d: %d, i: %d\n", s, d, i);
    return (s % 2 != 0 || d % 2 != 0);
}

char *readline_open_quotes(char *str)
{
    char *tmp;
    char *res;

    tmp = readline("quote> ");
    if (!tmp)
    {
        free(str); // Free the original string if readline fails
        return (NULL);
    }
    res = ft_strjoin_buffer(str, tmp, '\n');
    free(str); // Free the original string after joining
    free(tmp); // Free the temporary string
    if (!res)
        return (NULL);
    return (res);
}

