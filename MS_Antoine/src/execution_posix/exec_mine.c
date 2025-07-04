/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_mine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/04 16:52:27 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_output_redir(ASTNode *redir)
{
	int fd;

	if (!ft_strcmp(redir->value, ">"))
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->target->value);
		exit(1);
	}
	return (fd);
}

static void	process_output_redirs(ASTNode *node, int *fd)
{
	ASTNode *redir;
	int i;

	i = 0;
	while (i < node->child_count)
	{
		redir = node->children[i];
		while (redir && redir->type == NODE_REDIRECTION &&
			   (!ft_strcmp(redir->value, ">") || !ft_strcmp(redir->value, ">>")))
		{
			if (*fd != -1)
				close(*fd);
			*fd = open_output_redir(redir);
			redir = redir->left;
		}
		i++;
	}
}

static void	apply_output_redirections(ASTNode *node)
{
	int fd = -1;
	int i;
	ASTNode *child;

	for (i = 0; i < node->child_count; i++)
	{
		child = node->children[i];
		if (child->type == NODE_REDIRECTION &&
		   (!ft_strcmp(child->value, ">") || !ft_strcmp(child->value, ">>")))
		{
			if (fd != -1)
				close(fd);
			fd = open_output_redir(child);
		}
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

static void	process_input_redirs(ASTNode *node)
{
	ASTNode *redir;
	int fd;
	int i;

	i = 0;
	while (i < node->child_count)
	{
		redir = node->children[i];
		while (redir && redir->type == NODE_REDIRECTION)
		{
			if (!strcmp(redir->value, "<"))
			{
				fd = open_input_redir(redir);
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			redir = redir->left;
		}
		i++;
	}
}

static void	apply_input_redirections(ASTNode *node)
{
	int i;
	ASTNode *child;
	int fd;

	for (i = 0; i < node->child_count; i++)
	{
		child = node->children[i];
		if (child->type == NODE_REDIRECTION && !strcmp(child->value, "<"))
		{
			fd = open_input_redir(child);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
}

/// @brief calling the redirection functions
/// @param node Tree with the cmd
/// @param env 
void	apply_redirections(ASTNode *node, t_ast **env)
{
	apply_input_redirections(node);
	apply_output_redirections(node);
}

/// @brief executes the command with execve, redirections are made just before
/// @param node 
/// @param env 
void	exec_cmd(ASTNode *node, t_ast **env)
{
	int		i;
	int		argc;
	char	**tab;

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
	apply_redirections(node, env);
	if (cmd(tab, env))
	{
		printf("EXECVE IS RUNNING\n");
		execve(get_cmd_path(tab[0], env), tab, (*env)->env->env);
		perror("execve");
	}
	exit(1);
}
/// @brief main function that will take the ast node and take care of the execution
/// @param head ASTNode containing the tree
/// @param env 
void	execute_nodes(ASTNode **head, t_ast **env)
{
    if (!head || !(*head))
        return;
    exec_ast(*head, env, STDIN_FILENO, STDOUT_FILENO);
}
