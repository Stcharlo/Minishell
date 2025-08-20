/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_recursion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:28:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/20 16:37:55 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void free_env_variables(char **env)
{
    int i = 0;

    if (!env)
        return;

    while (env[i])
    {
        free(env[i]); // Free each string
        i++;
    }
    free(env); // Free the array itself
}

void	exec_pipe_right(ASTNode *node, t_ast **env, int output_fd, int *fd)
{
    int error_code; // Store the error code before freeing
    
    if (fd[0] != STDIN_FILENO)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (output_fd != STDOUT_FILENO)
		dup2(output_fd, STDOUT_FILENO);
	close(fd[1]);
	exec_ast(node->right, env, STDIN_FILENO, output_fd);
	error_code = (*env)->env->error_code;
	ast_free(node);
	free_env_complete(*env);
	//free_env_variables((*env)->env->env);
	
	exit(error_code);
}

void	exec_pipe_left(ASTNode *node, t_ast **env, int input_fd, int *fd)
{
    int error_code;
    
    if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (fd[1] != STDOUT_FILENO)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	close(fd[0]);
	exec_ast(node->left, env, input_fd, STDOUT_FILENO);
	error_code = (*env)->env->error_code;
	ast_free(node);
	free_env_complete(*env);
	//free_env_variables((*env)->env->env);
	exit(error_code);
}

/// @brief recursive function that will check if the node is a pipe or a command
/// @param node
/// @param env
/// @param input_fd our current input fd
/// @param output_fd our current output fd
void	exec_ast(ASTNode *node, t_ast **env, int input_fd, int output_fd)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
		exec_pipe_node(node, env, input_fd, output_fd);
	else if (node->type == NODE_COMMAND)
		exec_command_node(node, env, input_fd, output_fd);
}
