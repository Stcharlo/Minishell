/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:04:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/09 11:28:25 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

ASTNode	*create_ast_node(int type, char *value)
{
	ASTNode	*new;

	new = malloc(sizeof(ASTNode));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = strdup(value);
	if (!new->value)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	new->children = NULL;
	new->child_count = 0;
	new->target = NULL;
	return (new);
}

void	add_ast_child(ASTNode *parent, ASTNode *child)
{
	ASTNode	**new_children;
	int		i;

	i = 0;
	if (!parent || !child)
		return ;
	new_children = malloc(sizeof(ASTNode *) * (parent->child_count + 1));
	if (!new_children)
		return ;
	while (i < parent->child_count)
	{
		new_children[i] = parent->children[i];
		i++;
	}
	new_children[parent->child_count] = child;
	free(parent->children);
	parent->children = new_children;
	parent->child_count++;
	child->parent = parent;
}

static char	*get_path_var(t_ast **env)
{
	t_ast	*current;
	int		i;

	current = *env;
	i = 0;
	while (current->env->env[i])
	{
		if (strncmp(current->env->env[i], "PATH=", 5) == 0)
			return (current->env->env[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, const char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_slash(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*get_cmd_path(const char *cmd, t_ast **env)
{
	char	*path_var;
	char	**paths;

	if (strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_path_var(env);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ":");
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
