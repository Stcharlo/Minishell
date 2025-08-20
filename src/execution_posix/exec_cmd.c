/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 11:36:43 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/20 14:56:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**build_command_args(ASTNode *node)
{
	char	**tab;
	int		i;
	int		argc;

	tab = malloc(sizeof(char *) * (node->child_count + 2));
	if (!tab)
		return (NULL);
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
	return (tab);
}

static void	handle_path_errors(char *path, char **tab, t_ast **env, int child)
{
	if (path == NULL || 0 > access(path, F_OK))
	{
		command_not_found_error(tab[0]);
		(*env)->env->error_code = 127;
		exit_child((*env)->env->error_code, child);
		return ;
	}
	else if (is_directory(path) || access(path, X_OK) != 0)
	{
		(*env)->env->error_code = 126;
		exit_child((*env)->env->error_code, child);
		return ;
	}
}

static void	execute_command(char **tab, char *path, t_ast **env, int child)
{
	handle_path_errors(path, tab, env, child);
	if (execve(path, tab, (*env)->env->env) < 0)
	{
		free(path);
		free_tab(tab);
		(*env)->env->error_code = 1;
		exit_child((*env)->env->error_code, child);
		return ;
	}
	free(path);
	free_tab(tab);
	(*env)->env->error_code = 0;
	exit_child((*env)->env->error_code, child);
}

/// @brief executes the command with execve, redirections are made just before
/// @param node
/// @param env
void	exec_cmd(ASTNode *node, t_ast **env, int child)
{
	char	**tab;
	char	*path;

	if (!node)
		return ;
	tab = build_command_args(node);
	if (!tab)
		return ;
	if (apply_redirections(node) == -1)
	{
		(*env)->env->error_code = 1;
		exit_child((*env)->env->error_code, child);
		return ;
	}
	path = get_cmd_path(tab[0], env);
	if (cmd(tab, path, env))
	{
		(*env)->env->error_code = 0;
		execute_command(tab, path, env, child);
	}
}
