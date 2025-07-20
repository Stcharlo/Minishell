/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd7.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:21:38 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/20 18:24:04 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	env_recognition(char **tab, int j, t_ast **env)
{
	int		i;
	t_ast	*current;

	current = *env;
	i = 0;
	if (!current->env->env)
		return ;
	if (tab[j + 1])
		return ;
	while (current->env->env[i])
	{
		printf("%s\n", current->env->env[i]);
		i++;
	}
	return ;
}

void	pwd_recognition(t_ast **env)
{
	int		i;
	int		j;
	t_ast	*current;

	i = 0;
	j = 4;
	current = *env;
	if (!current->env->env[i])
		return ;
	while (current->env->env[i] && strncmp(current->env->env[i], "PWD=", 4))
		i++;
	if (current->env->env[i] && !strncmp(current->env->env[i], "PWD=", 4))
	{
		while (current->env->env[i][j] != '\0')
		{
			write(1, &current->env->env[i][j], 1);
			j++;
		}
		printf("\n");
		i++;
	}
	return ;
}

int	cmd(char **tab, char *path, t_ast **env)
{
	int	i;

	i = 0;
	if (ft_strnstr(BUILTIN, tab[i]) == 1)
	{
		build_in(tab, i, env);
		free(tab);
		free(path);
		return (0);
	}
	return (1);
}

int	cmd_recognize(char *tab)
{
	int	i;

	i = 0;
	if (ft_strnstr(BUILTIN, tab) == 1)
	{
		return (0);
	}
	return (1);
}

void	initialise_env(t_ast **env, char **envp)
{
	t_ast	*current;
	int		i;

	i = 0;
	if (!envp)
		return ;
	*env = malloc(sizeof(t_ast));
	(*env)->env = malloc(sizeof(t_env));
	current = *env;
	while (envp[i])
		i++;
	current->env->env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		current->env->env[i] = ft_strdup(envp[i]);
		i++;
	}
	current->env->env[i] = NULL;
	return ;
}
