/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_recursion2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:52:35 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 12:06:03 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

static void	setup_child_fds(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

static void	handle_child_process(ASTNode *node, t_ast **env, int input_fd,
		int output_fd)
{
	setup_child_fds(input_fd, output_fd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	exec_cmd(node, env, 1);
	exit(0);
}

static void	handle_parent_process(pid_t pid, t_ast **env)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		(*env)->env->error_code = WEXITSTATUS(status);
		(*env)->env->last_pid = pid;
	}
	else if (WIFSIGNALED(status))
	{
		g_exit_code = 128 + WTERMSIG(status);
		(*env)->env->error_code = g_exit_code;
	}
}

void	exec_command_node(ASTNode *node, t_ast **env, int input_fd,
		int output_fd)
{
	pid_t	pid;

	if (!cmd_recognize(node->value))
	{
		exec_cmd(node, env, 0);
	}
	else
	{
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
			handle_child_process(node, env, input_fd, output_fd);
		else
			handle_parent_process(pid, env);
	}
	return ;
}

/// @brief calling the redirection functions
/// @param node Tree with the cmd
/// @param env
int	apply_redirections(ASTNode *node)
{
	int		i;
	ASTNode	*child;

	i = 0;
	while (i < node->child_count)
	{
		child = node->children[i];
		if (child->type == NODE_REDIRECTION)
		{
			if (process_redirection_child(child) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
