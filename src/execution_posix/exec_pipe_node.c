/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:58:44 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/20 16:07:54 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

static void	fork_and_exec_processes(ASTNode *node, t_ast **env,
		t_pipe_data *data)
{
	data->pids[1] = fork();
	if ((data->pids[1]) < 0)
		return (perror("fork"));
	if (data->pids[1] == 0)
		exec_pipe_right(node, env, data->output_fd, data->fd);
	data->pids[0] = fork();
	if ((data->pids[0]) < 0)
		return (perror("fork"));
	if (data->pids[0] == 0)
		exec_pipe_left(node, env, data->input_fd, data->fd);
}

static void	wait_and_handle_status(pid_t *pids, t_ast **env)
{
	int	status_left;
	int	status_right;

	waitpid(pids[0], &status_left, 0);
	waitpid(pids[1], &status_right, 0);
	if (WIFEXITED(status_right))
		(*env)->env->error_code = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
	{
		g_exit_code = 128 + WTERMSIG(status_right);
		(*env)->env->error_code = g_exit_code;
	}
}

/// @brief the function will pipe and fork before
/// calling the functions to exec the left and right side of the node
/// @param node its a pipe node
/// @param env
/// @param input_fd
/// @param output_fd
void	exec_pipe_node(ASTNode *node, t_ast **env, int input_fd, int output_fd)
{
	int			fd[2];
	pid_t		pids[2];
	t_pipe_data	data;

	if (pipe(fd) < 0)
		return (perror("pipe"));
	data.fd = fd;
	data.pids = pids;
	data.input_fd = input_fd;
	data.output_fd = output_fd;
	fork_and_exec_processes(node, env, &data);
	close(fd[0]);
	close(fd[1]);
	wait_and_handle_status(pids, env);
}
