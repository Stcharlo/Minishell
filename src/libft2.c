/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:41:26 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 14:42:05 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_buffer(char const *s1, char const *s2, char buffer)
{
	char	*str;
	size_t	s1_len;
	size_t	s2_len;
	int		i;

	i = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	str = malloc(sizeof(char) * (s1_len + s2_len + 2));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i++] = buffer;
	while (s2[i - s1_len - 1])
	{
		str[i] = s2[i - s1_len - 1];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static void	copy_str(char *dst, const char *src, int *i)
{
	while (src[*i])
	{
		dst[*i] = src[*i];
		(*i)++;
	}
}

static void	copy_str2(char *dst, const char *src, int i)
{
	int	j;

	j = 0;
	while (src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	s1_len;
	size_t	s2_len;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	str = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (NULL);
	i = 0;
	copy_str(str, s1, &i);
	copy_str2(str, s2, i);
	return (str);
}
