/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/02 11:26:04 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;


void command_not_found_error(const char *cmd)
{
	fprintf(stderr, "%s: command not found\n", cmd);
}

void handle_errno_error(const char *path)
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

static int	open_output_redir(ASTNode *redir)
{
	int fd;

	if (!ft_strcmp(redir->value, ">"))
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		handle_errno_error(redir->target->value);
	return (fd);
}

static void	apply_output_redirections(ASTNode *node)
{
	int fd;
	int i;
	ASTNode *child;

	i = 0;
	fd = -1;
	while (i < node->child_count)
	{
		child = node->children[i];
		if (child->type == NODE_REDIRECTION &&
		   (!ft_strcmp(child->value, ">") || !ft_strcmp(child->value, ">>")))
		{
			if (fd != -1)
				close(fd);
			fd = open_output_redir(child);
		}
		i++;
	}
	if (fd != -1)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

/// @brief checking if node contains an input redirection and applying it to STDIN
/// @param node 
static int	open_input_redir(ASTNode *redir)
{
	int fd;

	fd = open(redir->target->value, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	return (fd);
}

static void	apply_input_redirections(ASTNode *node)
{
	int i;
	ASTNode *child;
	int fd;

	i = 0;
	while (i < node->child_count)
	{
		child = node->children[i];
		if (child->type == NODE_REDIRECTION && (!strcmp(child->value, "<") || !strcmp(child->value, "<<")))
		{
			fd = open_input_redir(child);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		i++;
	}
}

/// @brief calling the redirection functions
/// @param node Tree with the cmd
/// @param env 
void	apply_redirections(ASTNode *node)
{
	apply_input_redirections(node);
	apply_output_redirections(node);
}

int is_directory(const char *path)
{
	struct stat st;
	if (stat(path, &st) == 0)
		return S_ISDIR(st.st_mode);
	return 0;
}

void exit_child(int exit_code, int child)
{
	if (child == CHILD)
		exit (exit_code);
}

/// @brief executes the command with execve, redirections are made just before
/// @param node 
/// @param env 
void	exec_cmd(ASTNode *node, t_ast **env, int child)
{
	int		i;
	int		argc;
	char	**tab;
	char	*path;

	if (!node)
		return ;
	tab = malloc(sizeof(char *) * (node->child_count + 2));
	tab[0] = node->value;
	i = 0;
	argc = 1;
	while (i < node->child_count)
	{
		if (node->children[i]->type == NODE_ARGUMENT)
			tab[argc++] = node->children[i]->value;
		i++;
	}
	tab[argc] = 0;
	apply_redirections(node);
	path = get_cmd_path(tab[0], env);
	if (cmd(tab, path, env))
	{
		(*env)->env->error_code = 0;
		if (path == NULL || 0 > access(path, F_OK))
		{
			command_not_found_error(tab[0]);
		 (*env)->env->error_code = 127; // Command not found
			exit_child ((*env)->env->error_code, child);
			return;
		}
		else if (is_directory(path) || access(path, X_OK) != 0) 
		{
		 (*env)->env->error_code = 126; // Permission denied
			exit_child ((*env)->env->error_code, child);
			return;
		}
		else if (execve(path, tab, (*env)->env->env) < 0)
		{
		 (*env)->env->error_code = 1; // Command found but cannot execute
			exit_child ((*env)->env->error_code, child);
			return;
		}
	 (*env)->env->error_code = 0;
		exit_child ((*env)->env->error_code, child);
		return;
	}
	
}
/// @brief main function that will take the ast node and take care of the execution
/// @param head ASTNode containing the tree
/// @param env 
void	execute_nodes(ASTNode **head, t_ast **env)
{
	int saved_stdout;
	int saved_stdin;
	
	if (!head || !(*head))
		return;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	exec_ast(*head, env, STDIN_FILENO, STDOUT_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}
