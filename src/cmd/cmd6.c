/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:10:14 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/16 18:54:09 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	cd_recognition(char **tab, int i, t_ast **env)
{
	char	*buffer;
	char	*buffer2;

	buffer = malloc(1024);
	buffer2 = malloc(1024);
	if (!buffer || !buffer2)
	{
		free_buffer(buffer, buffer2, env);
		return ;
	}
	if (tab[i])
		i++;
	if (!tab[i])
	{
		cd_only(tab, i, env);
		return ;
	}
	if (tab[i + 1])
		free_tab1(buffer, buffer2);
	if (access_error(tab[i]) == 0)
		cd_rec_fnc(tab[i], buffer, buffer2, env);
	print_error(access_error(tab[i]), tab[i], env);
	free(buffer);
	free(buffer2);
	return ;
}

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
	char *copy;

	i = 0;
	a = 0;
	copy = malloc(1024);
	if (!copy)
		return NULL;
	j = strlen(str);
	if (!(*env)->env->env[i])
		return NULL;
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

void	cd_exit_code(void)
{
	g_exit_code = 1;
	exit(g_exit_code);
}

void	print_error(int num, char *tab, t_ast **env)
{
	if (!env || !*env || !(*env)->env)
		return;
	
	if (num == 1)
	{
		if (!tab)
			fprintf(stderr, "cd: No such file or directory\n");
		else
			fprintf(stderr, "cd: %s: No such file or directory\n", tab);
		(*env)->env->error_code = 1;
		return;
	}
	if (num == 2)
	{
		fprintf(stderr, "cd: %s: Permission denied\n", tab);
		(*env)->env->error_code = 1;
		return ;
	}
	return ;
}

int	access_error(char *tab)
{
	if (!tab)
		return (1);
	if (access(tab, F_OK) != 0)
		return (1);
	if (access(tab, R_OK) != 0)
		return (2);
	return (0);
}

void	pwd_change(char *pwd, char *oldpwd, t_ast **env)
{
	char	*pw;
	char	*oldpw;

	if (!env || !*env || !(*env)->env || !pwd || !oldpwd)
		return;
	
	unset_env("OLDPWD=", env);
	unset_exp("OLDPWD=", env);
	unset_env("PWD=", env);
	unset_exp("PWD=", env);
	
	pw = ft_strjoin("PWD=", pwd);
	oldpw = ft_strjoin("OLDPWD=", oldpwd);
	
	if (pw)
	{
		add_env(pw, env);
		add_export(pw, env);
		free(pw);
	}
	
	if (oldpw)
	{
		add_env(oldpw, env);
		add_export(oldpw, env);
		free(oldpw);
	}
	
	return;
}
