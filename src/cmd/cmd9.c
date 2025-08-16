/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd9.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:00:32 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/16 19:05:25 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	unset_exp_fnc(t_ast *current, char *target, char **temp, int j)
{
	int	count;

	count = 0;
	while (current->env->export && current->env->export[j])
	{
		if (ft_strncmp(current->env->export[j], (target), strlen(target)) != 0)
			temp[count++] = current->env->export[j];
		else if (ft_strncmp(current->env->export[j],
				(target), strlen(target)) == 0)
			free(current->env->export[j]);
		j++;
	}
	temp[count] = NULL;
	free_both(target, current);
	current->env->export = temp;
}

void	unset_env_fnc(t_ast *current, char *argv, char **temp, int j)
{
	int	count;

	count = 0;
	while (current->env->env[j])
	{
		if (ft_strncmp(current->env->env[j], argv, strlen(argv)) != 0)
			temp[count++] = current->env->env[j];
		else if (ft_strncmp(current->env->env[j], argv, strlen(argv)) == 0)
			free(current->env->env[j]);
		j++;
	}
	temp[count] = NULL;
	free(current->env->env);
	current->env->env = temp;
	return ;
}

void	add_env_fnc(t_ast *current, char **temp, char *argv)
{
	int	i;

	i = 0;
	while (current->env->env && current->env->env[i])
	{
		temp[i] = current->env->env[i];
		i++;
	}
	temp[i] = ft_strdup(argv);
	temp[i + 1] = NULL;
	free(current->env->env);
	current->env->env = temp;
	return ;
}

void	add_exp_fnc(t_ast *current, char **temp, char *argv)
{
	int	i;

	i = 0;
	while (current->env->export && current->env->export[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	if (!temp)
		return ;
	while (current->env->export && current->env->export[i])
	{
		temp[i] = current->env->export[i];
		i++;
	}
	current->env->error_code = 0;
	temp[i] = cat_dup(argv);
	temp[i + 1] = NULL;
	free(current->env->export);
	current->env->export = temp;
	return ;
}

void	cd_exit_code(void)
{
	g_exit_code = 1;
	exit(g_exit_code);
}