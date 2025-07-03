/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_mine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/03 19:18:55 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/// @brief finding last output redirection that needs to be applied
/// @param node 
/// @return 
static ASTNode	*find_last_output_redir(ASTNode *node)
{
	ASTNode	*redir;
	ASTNode	*last;
	int		i;

	last = NULL;
	i = 0;
	while (i < node->child_count)
	{
		redir = node->children[i];
		while (redir && redir->type == NODE_REDIRECTION &&
			   (!ft_strcmp(redir->value, ">") || !ft_strcmp(redir->value, ">>")))
		{
			last = redir;
			redir = redir->left;
		}
		i++;
	}
	return (last);
}
/// @brief checking if node contains an output redirection and applying it to STDOUT
/// @param node 
static void	apply_output_redirections(ASTNode *node)
{
	ASTNode	*last;
	int		fd;

	last = find_last_output_redir(node);
	if (!last)
		return ;
	if (!ft_strcmp(last->value, ">"))
		fd = open(last->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(last->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(last->target->value);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/// @brief checking if node contains an input redirection and applying it to STDIN
/// @param node 
static void	apply_input_redirections(ASTNode *node)
{
	ASTNode	*redir;
	int		fd;
	int		i;

	i = 0;
	while (i < node->child_count)
	{
		redir = node->children[i];
		while (redir && redir->type == NODE_REDIRECTION)
		{
			if (!strcmp(redir->value, "<"))
			{
				fd = open(redir->target->value, O_RDONLY);
				if (fd < 0)
				{
					perror("open");
					exit(1);
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			redir = redir->left;
		}
		i++;
	}
}

/// @brief calling the redirection functions
/// @param node Tree with the cmd
/// @param env 
void	apply_redirections(ASTNode *node, char **env)
{
	apply_input_redirections(node);
	apply_output_redirections(node);
}

/// @brief executes the command with execve, redirections are made just before
/// @param node 
/// @param env 
void	exec_cmd(ASTNode *node, char **env)
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
	execve(get_cmd_path(tab[0], env), tab, env);
	perror("execve");
	exit(1);
}
/// @brief main function that will take the ast node and take care of the execution
/// @param head ASTNode containing the tree
/// @param env 
void	execute_nodes(ASTNode **head, char **env)
{
    if (!head || !(*head))
        return;
    exec_ast(*head, env, STDIN_FILENO, STDOUT_FILENO);
}
