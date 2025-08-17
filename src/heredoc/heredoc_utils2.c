/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:41:09 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 12:45:27 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tab_to_file(char **lines, const char *filename)
{
	int		fd;
	char	temp_path[256];

	fd = open_tempfile(filename, temp_path, sizeof(temp_path));
	if (fd < 0)
		return (free_tab(lines));
	if (write_all_lines(fd, lines) < 0)
	{
		close(fd);
		return (free_tab(lines));
	}
	close(fd);
	link_or_copy_temp(temp_path, filename);
	free_tab(lines);
}

void	clean_heredoc(char **argv)
{
	char	temp_path[256];
	char	cmd[300];

	if (!ft_strcmp(argv[1], "here_doc"))
		free(argv[1]);
	get_next_line(-1);
	snprintf(temp_path, sizeof(temp_path), "/tmp/minishell_heredoc_*_%d",
		getpid());
	snprintf(cmd, sizeof(cmd), "rm -f %s 2>/dev/null", temp_path);
	system(cmd);
}

void	check_heredoc(t_token **lst, t_ast **env)
{
	t_token		*list;
	extern int	g_exit_code;
	int			saved_exit_code;

	saved_exit_code = g_exit_code;
	list = *lst;
	while (list)
	{
		if (list->type == HEREDOC)
		{
			if (list->next && list->next->value)
				start_heredoc(list->next->value, list->next->was_quoted, env);
			if (g_exit_code == 130)
				break ;
		}
		list = list->next;
	}
	if (g_exit_code != 130)
		g_exit_code = saved_exit_code;
}

int	is_limiter_line(char *line, char *limiter)
{
	return (ft_strcmp(line, limiter) == 0);
}
