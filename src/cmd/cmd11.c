/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd11.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:56:59 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/18 16:16:08 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	free_env_complete(t_ast *env)
{
	int	i;

	i = 0;
	if (!env || !env->env)
		return ;
	if (env->env->env)
		free_env_env(env);
	if (env->env->export)
	{
		while (env->env->export[i])
		{
			if (env->env->export[i])
				free(env->env->export[i]);
			i++;
		}
		free(env->env->export);
	}
	free(env->env);
	free(env);
}

void	free_env_env(t_ast *env)
{
	int	i;

	i = 0;
	while (env->env->env[i])
	{
		if (env->env->env[i])
			free(env->env->env[i]);
		i++;
	}
	free(env->env->env);
}

void	too_much_exit(t_ast **env)
{
	(*env)->env->error_code = 1;
	exit((*env)->env->error_code);
}

void	number_has_sign(char **argv, int i, t_ast **env)
{
	(*env)->env->error_code = (256 + atoi(argv[i + 1])) % 256;
	num_has_sign(env);
}

void	number_not_valid(char **argv, int i, t_ast **env)
{
	(*env)->env->error_code = 2;
	valid_number_fail(env, argv[i + 1]);
}
