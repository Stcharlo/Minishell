/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:10:14 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/02 11:46:59 by agaroux          ###   ########.fr       */
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
	if (tab[i])
		i++;
	if (tab[i + 1])
		cd_exit_code();
	error_type = access_error(tab[i]);
	if (error_type == 0)
	{
		oldpwd = getcwd(buffer2, 1024);
		if (chdir(tab[i]) != 0)
			return ;
		pwd = getcwd(buffer, 1024);
		pwd_change(pwd, oldpwd, env);
		(*env)->env->error_code = 0;
	}
	print_error(error_type, tab[i], env);
	free(buffer);
	free(buffer2);
	return ;
}

void	cd_exit_code(void)
{
	g_exit_code = 1;
	exit(g_exit_code);
}

void	print_error(int num, char *tab, t_ast **env)
{
	if (num == 1)
	{
		strerror(errno);
		(*env)->env->error_code = 1;
		return ;
	}
	if (num == 2)
	{
		printf("cd: %s: Permission denied\n", tab);
		(*env)->env->error_code = 1;
		return ;
	}
	return ;
}

int	access_error(char *tab)
{
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

	unset_env("OLDPWD=", env);
	unset_exp("OLDPWD=", env);
	unset_env("PWD=", env);
	unset_exp("PWD=", env);
	pw = ft_strjoin("PWD=", pwd);
	oldpw = ft_strjoin("OLDPWD=", oldpwd);
	add_env(pw, env);
	add_env(oldpw, env);
	add_export(pw, env);
	add_export(oldpw, env);
	if (pw)
		free(pw);
	if (oldpw)
		free(oldpw);
	return ;
}
