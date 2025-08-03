/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_recursion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:28:00 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/03 06:58:34 by agaroux          ###   ########.fr       */
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
    // Exit with the actual error code, not 0
    exit((*env)->env->error_code);
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
    // Exit with the actual error code, not 0
    exit((*env)->env->error_code);
}

void exec_command_node(ASTNode *node, t_ast **env, int input_fd, int output_fd)
{
    int status;
    pid_t pid;

    if (!cmd_recognize(node->value)) {
        // Builtin: run in parent, set g_exit_code only if builtin sets it
        exec_cmd(node, env, 0);
        // Builtins should set g_exit_code themselves on error
    } else {
        // External command: run in child, set g_exit_code from child exit status
        if ((pid = fork()) < 0)
            return;
        if (pid == 0) {
            if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            exec_cmd(node, env, 1);
            // Do not set g_exit_code in child, just exit with correct code
            exit(0);
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                // Store normal exit codes in the env structure, not in g_exit_code
                (*env)->env->error_code = WEXITSTATUS(status);
                (*env)->env->last_pid = pid;
            } else if (WIFSIGNALED(status)) {
                // Only use g_exit_code for signal-related exits
                g_exit_code = 128 + WTERMSIG(status);
                (*env)->env->error_code = g_exit_code;
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
    int status_left, status_right;
    pid_t left_pid, right_pid;

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
    waitpid(left_pid, &status_left, 0);
    waitpid(right_pid, &status_right, 0);
    // Set the exit code of the last command in the pipeline
    if (WIFEXITED(status_right)) {
        // Store normal exit codes in the env structure, not in g_exit_code
        (*env)->env->error_code = WEXITSTATUS(status_right);
    } else if (WIFSIGNALED(status_right)) {
        // Only use g_exit_code for signal-related exits
        g_exit_code = 128 + WTERMSIG(status_right);
        (*env)->env->error_code = g_exit_code;
    }
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

