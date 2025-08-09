/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:30:17 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 12:10:52 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_heredoc(t_token **lst)
{
	t_token	*list;

	list = *lst;
	while (list)
	{
		if (list->type == HEREDOC)
		{
			if (list->next && list->next->value)
				start_heredoc(list->next->value);
		}
		list = list->next;
	}
}

void	start_heredoc(char *limiter)
{
	read_heredoc(limiter);
}

void	tab_to_file(char **lines, const char *filename)
{
	int		fd;
	int		i;
	ssize_t	written;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return ;
	}
	i = 0;
	while (lines && lines[i])
	{
		written = write(fd, lines[i], ft_strlen(lines[i]));
		if (written < 0)
		{
			perror("write");
			close(fd);
			return ;
		}
		write(fd, "\n", 1);
		i++;
	}
	close(fd);
	free_tab(lines);
}

void	clean_heredoc(char **argv)
{
	if (!ft_strcmp(argv[1], "here_doc"))
		free(argv[1]);
	get_next_line(-1);
	unlink("here_doc");
}

void	read_heredoc(char *limiter)
{
	char	*line;
	char	*str;
	char	**res;

	str = ft_strdup("");
	if (!str)
		return ;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 10)
		{
			free(line);
			break ;
		}
		str = ft_strjoin(str, line);
		free(line);
	}
	res = ft_split(str, "\n");
	free(str);
	tab_to_file(res, limiter);
}
