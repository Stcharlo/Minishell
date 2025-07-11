/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_recursion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:28:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/11 15:27:34 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_exit_code;

void exec_pipe_right(ASTNode *node, t_ast **env, int output_fd, int *fd)
{
    if (fd[0] != STDIN_FILENO)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
    }
    if (output_fd != STDOUT_FILENO)
        dup2(output_fd, STDOUT_FILENO);
    close(fd[1]);
    exec_ast(node->right, env, STDIN_FILENO, output_fd);
    exit(0);
}

void exec_pipe_left(ASTNode *node, t_ast **env, int input_fd, int *fd)
{
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
    exit(0);
}

void exec_command_node(ASTNode *node, t_ast **env, int input_fd, int output_fd)
{
    int status;
    pid_t pid;

    if (!cmd_recognize(node->value))
        exec_cmd(node, env, 0);
    else
    {
        if ((pid = fork()) < 0)
            return;
        if (pid == 0)
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
            exec_cmd(node, env, 1);
            exit(0);
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                g_exit_code = WEXITSTATUS(status);
                (*env)->env->last_pid = pid;
            }
        }
    }
    return;
}

/// @brief the function will pipe and fork before calling the functions to exec the left and right side of the node
/// @param node its a pipe node
/// @param env 
/// @param input_fd 
/// @param output_fd 
void exec_pipe_node(ASTNode *node, t_ast **env, int input_fd, int output_fd)
{
    int fd[2];
    int status;
    pid_t left_pid;
    pid_t right_pid;

    if (pipe(fd) < 0)
        return (perror("pipe"));
    if ((right_pid = fork()) < 0)
        return (perror("fork"));
    if (right_pid == 0)
        exec_pipe_right(node, env, output_fd, fd);
    if ((left_pid = fork()) < 0)
        return (perror("fork"));
    if (left_pid == 0)
        exec_pipe_left(node, env, input_fd, fd);
    close(fd[0]);
    close(fd[1]);
    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);
}
/// @brief recursive function that will check if the node is a pipe or a command
/// @param node 
/// @param env 
/// @param input_fd our current input fd
/// @param output_fd our current output fd
void exec_ast(ASTNode *node, t_ast **env, int input_fd, int output_fd)
{
    if (!node)
        return;
    if (node->type == NODE_PIPE)
        exec_pipe_node(node, env, input_fd, output_fd);
    else if (node->type == NODE_COMMAND)
        exec_command_node(node, env, input_fd, output_fd);
}

