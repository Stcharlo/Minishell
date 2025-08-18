/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd10.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:05:45 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/18 18:39:04 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void cd_only(t_ast **env)
{
    char *home = get_env_var(env, "HOME=");
    if (!home)
    {
        print_error(-1, "HOME not set", env);
        return;
    }
    // Appeler cd_rec_fnc directement au lieu de récursion
    cd_rec_fnc(home, env);
}


void	free_tab1(char *buffer, char *buffer2)
{
	free(buffer);
	free(buffer2);
	cd_exit_code();
}

void	cd_rec_fnc(char *tab, t_ast **env)
{
	char	*oldpwd;
	char	*pwd;
	char	*buffer;
	char	*buffer2;

	buffer = malloc(1024);
	buffer2 = malloc(1024);
	if (!buffer || !buffer2)
	{
		free_buffer(buffer, buffer2, env);
		return ;
	}
	oldpwd = getcwd(buffer2, 1024);
	if (chdir(tab) != 0)
	{
		free(buffer);
		free(buffer2);
		return ;
	}
	pwd = getcwd(buffer, 1024);
	pwd_change(pwd, oldpwd, env);
	free(buffer);
	free(buffer2);
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

char *get_env_var(t_ast **env, char *str)
{
    int i;
    int len;

    if (!env || !(*env) || !(*env)->env || !(*env)->env->env)
        return NULL;

    len = strlen(str);
    i = 0;
    while ((*env)->env->env[i])
    {
        if (strncmp((*env)->env->env[i], str, len) == 0)
            return (*env)->env->env[i] + len;  // pointeur directement après "str"
        i++;
    }
    return NULL;
}
