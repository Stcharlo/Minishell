/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:47:07 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/20 18:27:40 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	build_in(char **tab, int i, t_ast **env)
{
	if (ft_strnstr(BUILTIN, tab[i]))
	{
		if (ft_strnstr("pwd", tab[i]))
			pwd_recognition(env);
		if (ft_strnstr("env", tab[i]))
			env_recognition(tab, i, env);
		if (ft_strnstr("cd", tab[i]))
			cd_recognition(tab, i, env);
		if (ft_strnstr("export", tab[i]))
			export_recognition(tab, i, env);
		if (ft_strnstr("unset", tab[i]))
			unset_recognition(tab, i, env);
		if (ft_strnstr("echo", tab[i]))
			echo_recognition(tab, i);
		if (ft_strnstr("exit", tab[i]))
			exit_recognition(tab, i);
		return ;
	}
	return ;
}

void	initialise_exp(t_ast **env, char **envp)
{
	t_ast	*current;
	int		i;

	i = 0;
	if (!envp)
		return ;
	current = *env;
	while (envp[i])
		i++;
	current->env->export = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		current->env->export[i] = cat_dup(envp[i]);
		i++;
	}
	current->env->export[i] = NULL;
	return ;
}

void	initialise_shlvl(t_ast **env)
{
	char	*str;
	char	*merge;
	int		shlvl;
	char	*final;

	str = number_shlvl(env);
	if (!str)
		return ;
	shlvl = ft_atoi(str);
	free(str);
	shlvl++;
	final = ft_itoa(shlvl);
	unset_env("SHLVL=", env);
	unset_exp("SHLVL=", env);
	merge = ft_strjoin("SHLVL=", final);
	free(final);
	add_env(merge, env);
	add_export(merge, env);
	free(merge);
	return ;
}

char	*number_shlvl(t_ast **env)
{
	t_ast	*current;
	int		i;
	char	*str;
	char	*result;

	i = 0;
	current = *env;
	if (!current->env->env)
		return (NULL);
	while (current->env->env[i])
	{
		if (!(ft_strncmp(current->env->env[i], "SHLVL=", 6)))
		{
			str = current->env->env[i] + 6;
			while (*str && !ft_isdigit(*str))
				str++;
			result = ft_strdup(str);
			return (result);
		}
		i++;
	}
	return (ft_strdup("0"));
}

void	free_env_complete(t_ast *env)
{
	int	i;

	i = 0;
	if (!env || !env->env)
		return ;
	if (env->env->env)
	{
		while (env->env->env[i])
			free(env->env->env[i++]);
		free(env->env->env);
	}
	i = 0;
	if (env->env->export)
	{
		while (env->env->export[i])
			free(env->env->export[i++]);
		free(env->env->export);
	}
	free(env->env);
	free(env);
}
