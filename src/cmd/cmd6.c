/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:10:14 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/10 15:32:18 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	cd_recognition(char **tab, int i, t_ast **env)
{
	char	*buffer;
	char	*oldpwd;
	char	*pwd;
	char	*buffer2;
	int		error_type;

	buffer = malloc(1024);
	buffer2 = malloc(1024);
	if (!buffer || !buffer2)
	{
		if (buffer) free(buffer);
		if (buffer2) free(buffer2);
		(*env)->env->error_code = 1;
		return;
	}
	
	// Skip "cd" command
	if (tab[i])
		i++;
	
	// Handle case where no directory is specified
	if (!tab[i])
	{
		char *home = getenv("HOME");
		if (home)
		{
			oldpwd = getcwd(buffer2, 1024);
			if (chdir(home) == 0)
			{
				pwd = getcwd(buffer, 1024);
				pwd_change(pwd, oldpwd, env);
				(*env)->env->error_code = 0;
			}
		}
		free(buffer);
		free(buffer2);
		return;
	}
	
	// Too many arguments
	if (tab[i + 1])
	{
		free(buffer);
		free(buffer2);
		cd_exit_code();
	}
	
	error_type = access_error(tab[i]);
	if (error_type == 0)
	{
		oldpwd = getcwd(buffer2, 1024);
		if (chdir(tab[i]) != 0)
		{
			free(buffer);
			free(buffer2);
			return;
		}
		pwd = getcwd(buffer, 1024);
		pwd_change(pwd, oldpwd, env);
		(*env)->env->error_code = 0;
	}
	print_error(error_type, tab[i], env);
	free(buffer);
	free(buffer2);
	return;
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
		return;
	}
	return;
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
