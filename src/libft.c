/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:41:26 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/02 13:02:15 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = malloc(nmemb * size);
	if (!ptr)
		return (0);
	else
	{
		i = 0;
		while (i < (size * nmemb))
		{
			ptr[i] = 0;
			i++;
		}
		return (ptr);
	}
}

char	*ft_strdup(const char *s1)
{
	char	*dst;
	size_t	i;

	dst = (char *)malloc(ft_strlen(s1) + 1);
	if (!dst)
		return (0);
	i = 0;
	while (s1[i])
	{
		dst[i] = s1[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t		i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i] && i < n)
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	i;
	size_t	str_len;

	if (!s)
		return (0);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	result = (char *)ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (0);
	i = 0;
	while (i < len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = 0;
	return (result);
}

size_t	ft_strlen(char const *src)
{
	size_t	i;

	i = 0;
	while (src[i])
		i++;
	return (i);
}

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
	while (s2[i - s1_len-1])
	{
		str[i] = s2[i - s1_len-1];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	s1_len;
	size_t	s2_len;
	int		i;

	if (!s1 || !s2)
		return NULL;
	
	i = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	
	str = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (NULL);
	
	while (s1[i]) {
		str[i] = s1[i];
		i++;
	}
	
	int j = 0;
	while (s2[j]) {
		str[i + j] = s2[j];
		j++;
	}
	
	str[i + j] = '\0';
	return (str);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	src_len = 0;
	while (src[src_len])
		src_len++;
	if (dstsize == 0)
		return (src_len);
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}
int	ft_isdigit(int i)
{
	if (i >= '0' && i <= '9')
		return (1);
	return (0);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char	un_c;
	int				i;

	un_c = (unsigned char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == un_c)
			return ((char *)(s + i));
		i++;
	}
	if (!un_c)
		return ((char *)(s + i));
	return (0);
}

int		ft_strnstr(char *big, char *little)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (*little == '\0')
		return (0);
	while (big[i])
	{
		j = 0;
		while (big[i + j] == little[j])
		{
			j++;
			if (little[j] == ':' || little[j] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] == s2[i] && s1[i] && s2[i])
        i++;
    return (s1[i] - s2[i]);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	c;
	int	resultat;

	i = 0;
	c = 1;
	resultat = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			c = -c;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		resultat = (resultat * 10) + (nptr[i] - '0');
		i++;
	}
	return (resultat * c);
}

/**
 * Outputs the string 's' to the given file descriptor.
 * 
 * @param s The string to output.
 * @param fd The file descriptor on which to write.
 */
void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}
