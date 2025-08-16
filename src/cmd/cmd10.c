/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd10.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:05:45 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/16 19:06:55 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	cd_only(char **tab, int i, t_ast **env)
{
	tab[i] = get_env_var(env, "HOME=");
	tab[i + 1] = NULL;
	i--;
	cd_recognition(tab, i, env);
}

void	free_tab1(char *buffer, char *buffer2)
{
	free(buffer);
	free(buffer2);
	cd_exit_code();
}

void	cd_rec_fnc(char *tab, char *buffer, char *buffer2, t_ast **env)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = NULL;
	pwd = NULL;
	oldpwd = getcwd(buffer2, 1024);
	if (chdir(tab) != 0)
	{
		free(buffer);
		free(buffer2);
		return ;
	}
	pwd = getcwd(buffer, 1024);
	pwd_change(pwd, oldpwd, env);
	(*env)->env->error_code = 0;
	return ;
}

void	free_buffer(char *buffer, char *buffer2, t_ast **env)
{
	if (buffer)
		free(buffer);
	if (buffer2)
		free(buffer);
	(*env)->env->error_code = 1;
	return ;
}

char	*get_env_var(t_ast **env, char *str)
{
	int		i;
	int		j;
	int		a;
	char	*copy;

	i = 0;
	a = 0;
	copy = malloc(1024);
	if (!copy)
		return (NULL);
	j = strlen(str);
	if (!(*env)->env->env[i])
		return (NULL);
	while ((*env)->env->env[i] && strncmp((*env)->env->env[i], str, strlen(str)))
		i++;
	if ((*env)->env->env[i] && !strncmp((*env)->env->env[i], str, strlen(str)))
	{
		while ((*env)->env->env[i][j] != '\0')
			copy[a++] = (*env)->env->env[i][j++];
		i++;
	}
	return (strdup(copy));
}
