/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:16:42 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/08 13:05:51 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_replace(char *buffer)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	tmp = ft_calloc(sizeof(char), ft_strlen(buffer) - i);
	if (!tmp)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i])
		tmp[j++] = buffer[i++];
	free(buffer);
	return (tmp);
}

char	*ft_extract(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (!buffer || !buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(sizeof(char), i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}

char	*ft_buffer(int fd, char *buffer)
{
	char	*tmp;
	int		bytes;

	if (!buffer)
		buffer = ft_calloc(1, 1);
	if (!buffer)
		return (NULL);
	while (1)
	{
		tmp = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
		if (!tmp)
			return (NULL);
		bytes = read(fd, tmp, BUFFER_SIZE);
		if (bytes < 0)
			return (free(tmp), NULL);
		buffer = ft_strjoin(buffer, tmp);
		if (!buffer)
			return (NULL);
		free(tmp);
		if (bytes == 0 || ft_newline(buffer))
			break ;
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free(buffer), NULL);
	buffer = ft_buffer(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_extract(buffer);
	buffer = ft_replace(buffer);
	return (line);
}
