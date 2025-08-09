/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 11:37:28 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	command_not_found_error(const char *cmd)
{
	fprintf(stderr, "%s: command not found\n", cmd);
}

void	handle_errno_error(const char *path)
{
	if (errno == EACCES)
	{
		fprintf(stderr, "%s: Permission denied\n", path);
		exit(1);
	}
	else if (errno == ENOENT)
	{
		fprintf(stderr, "%s: No such file or directory\n", path);
		exit(1);
	}
	else
	{
		perror(path);
		exit(1);
	}
}

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

void	exit_child(int exit_code, int child)
{
	if (child == CHILD)
		exit(exit_code);
}

/// @brief main function that will take the ast
/// node and take care of the execution
/// @param head ASTNode containing the tree
/// @param env
void	execute_nodes(ASTNode **head, t_ast **env)
{
	int	saved_stdout;
	int	saved_stdin;

	if (!head || !(*head))
		return ;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	exec_ast(*head, env, STDIN_FILENO, STDOUT_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}
