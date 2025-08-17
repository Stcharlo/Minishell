/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:25:03 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 12:45:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*heredoc_readline(void)
{
	char	*line;

	write(1, "> ", 2);
	line = get_next_line(0);
	if (!line)
		return (NULL);
	line[strcspn(line, "\n")] = '\0';
	return (line);
}

static void	maybe_expand_line(char **line, int quoted_limiter, t_ast **env)
{
	char	*expanded;

	if (quoted_limiter)
		return ;
	expanded = unquoted_var_expansion(*line, env);
	if (expanded != *line)
	{
		free(*line);
		*line = expanded;
	}
}

static void	append_line(char **acc, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(*acc, line);
	free(*acc);
	*acc = tmp;
	tmp = ft_strjoin(*acc, "\n");
	free(*acc);
	*acc = tmp;
}

static void	finalize_heredoc(char *str, char *filename)
{
	extern int	g_exit_code;
	char		**res;

	if (g_exit_code != 130)
	{
		res = ft_split(str, "\n");
		free(str);
		tab_to_file(res, filename);
	}
	else
	{
		free(str);
		free(filename);
	}
}

void	read_heredoc(char *limiter, int quoted_limiter, t_ast **env)
{
	char	*line;
	char	*str;
	char	*temp_filename;

	str = ft_strdup("");
	if (!str)
		return ;
	temp_filename = ft_strdup(limiter);
	while (1)
	{
		line = heredoc_readline();
		if (!line)
			break ;
		if (is_limiter_line(line, limiter))
		{
			free(line);
			break ;
		}
		maybe_expand_line(&line, quoted_limiter, env);
		append_line(&str, line);
		free(line);
	}
	finalize_heredoc(str, temp_filename);
}
