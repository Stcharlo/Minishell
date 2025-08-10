/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd8.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 12:17:34 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 12:19:23 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_ast_node(t_ast *node)
{
	node->type = 0;
	node->value = NULL;
	node->left = NULL;
	node->right = NULL;
	node->children = NULL;
	node->child_count = 0;
	node->target = NULL;
	node->parent = NULL;
}

static void	init_env_struct(t_env *env_struct)
{
	env_struct->error_code = 0;
	env_struct->last_pid = 0;
	env_struct->export = NULL;
}

static int	allocate_ast_structures(t_ast **env)
{
	*env = malloc(sizeof(t_ast));
	if (!*env)
		return (0);
	(*env)->env = malloc(sizeof(t_env));
	if (!(*env)->env)
	{
		free(*env);
		return (0);
	}
	return (1);
}

static int	copy_env_variables(t_ast *current, char **envp, int env_count)
{
	int	i;

	current->env->env = malloc(sizeof(char *) * (env_count + 1));
	if (!current->env->env)
		return (0);
	i = 0;
	while (envp[i])
	{
		current->env->env[i] = ft_strdup(envp[i]);
		if (!current->env->env[i])
		{
			while (--i >= 0)
				free(current->env->env[i]);
			return (0);
		}
		i++;
	}
	current->env->env[i] = NULL;
	return (1);
}

void	initialise_env(t_ast **env, char **envp)
{
	t_ast	*current;
	int		env_count;

	if (!envp)
		return ;
	if (!allocate_ast_structures(env))
		return ;
	current = *env;
	env_count = 0;
	while (envp[env_count])
		env_count++;
	if (!copy_env_variables(current, envp, env_count))
	{
		free(current->env);
		free(*env);
		return ;
	}
	init_env_struct(current->env);
	init_ast_node(current);
}
