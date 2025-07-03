/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:47:07 by stcharlo          #+#    #+#             */
/*   Updated: 2025/06/29 19:03:02 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void Build_in(char **argv, int i, t_env **env)
{
    if (ft_strnstr(BUILTIN, argv[i]))
	{
		if (ft_strnstr("pwd", argv[i]))
			pwd_recognition(argv, env);
		if (ft_strnstr("env", argv[i]))
		 	env_recognition(argv, env);
		// if (ft_strnstr("echo", argv[i]))
		// 	echo_recognition(argv, env);
		if (ft_strnstr("cd", argv[i]))
            cd_recognition(argv, i);
		if (ft_strnstr("export", argv[i]))
		 	export_recognition(argv, i, env);
		if (ft_strnstr("unset", argv[i]))
            unset_recognition(argv, i, env);
		// if (ft_strnstr("exit", argv[i]))
		return ;
	}
	return ;
}

void unset_recognition(char **argv, int i, t_env **env)
{ 
    t_env *current;
    
    current = *env;
    i++;
    if (argv[i] == NULL)
        return ;
    while (argv[i])
    {
        unset_env(argv, i, env);
        unset_exp(argv, i, env);
        i++;
    }
    return ;
}

void unset_exp(char **argv, int i, t_env **env)
{
    t_env *current;
    char    **temp;
    int j;
    int count;

    j = 0;
    count = 0;
    current = *env;
    while (current->export[count])
        count++;
    temp = malloc(sizeof(char *) * (count + 1));
    count = 0;
    while (current->export[j])
    {
        if (ft_strncmp(current->export[j], argv[i], strlen(argv[i])))
            j++;
        temp[count] = current->export[j];
        count++;
        j++;
    }
    temp[count] = NULL;
    free(current->export);
    current->export = temp;
    show_export(env);
    return ;
}

void unset_env(char **argv, int i, t_env **env)
{
    t_env *current;
    char    **temp;
    int j;
    int count;

    j = 0;
    count = 0;
    current = *env;
    while (current->env[count])
        count++;
    temp = malloc(sizeof(char *) * (count));
    count = 0;
    while (current->env[j])
    {
        if (!strncmp(current->env[j], argv[i], strlen(argv[i])))
            j++;
        temp[count] = current->env[j];
        count++;
        j++;
    }
    temp[count] = NULL;
    free(current->env);
    current->env = temp;
    show_env(env);
    return ;
}

void export_recognition(char **argv, int i, t_env **env)
{
    t_env *current;
    int j;
    char **temp;
    
    j = 0;
    current = *env;
    i++;
    if (argv[i] == NULL)
        show_export(env);
    while (argv[i])
    {
        if (parse_exp(argv[i]) != 1 && !strchr(argv[i], '='))
            add_export(argv, i, env);
        if (parse_exp(argv[i]) != 1 && strchr(argv[i], '='))
        {
            add_env(argv, i, env);
            add_export(argv, i, env);
        }
        i++;
    }
    return ;
}
void    add_env(char **argv, int s, t_env **env)
{
    t_env *current;
    int i;
    char **temp;

    i = 0;
    current = *env;
    while(current->env[i])
        i++;
    temp = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while(current->env[i])
     {
        temp[i] = current->env[i];
        i++;
     }
     temp[i] = ft_strdup(argv[s]);
     temp[i + 1] = NULL;
    free(current->env);
    current->env = temp;
    show_env(env);
    return ;
}

int parse_exp(char *argv)
{
    int j;

    j = skip_isspace(argv);
    if (argv[j] >= '0' && argv[j] <= '9')
        return (1);
    if (check_dbl_equal(argv))
        return (1);
    while (isalpha(argv[j]) || isalnum(argv[j])|| argv[j] == '_')
        j++;
    if (argv[j] == '=' || argv[j] == '\0')
        return (0);
    if (argv[j] != '\0' || argv[j] != '=')
        return (1);
    return (1);
}

int check_dbl_equal(char *argv)
{
    int i;
    int temp;

    i = 0;
    temp = 0;
    
    while(argv[i])
    {
        if (argv[i] == '=')
            temp++;
        i++;
    }
    if (temp > 1)
        return (1);
    return (0);
}

int skip_isspace(char *argv)
{
    int i;

    i = 0;
    while (argv[i] == 32 || argv[i] == '\t' || argv[i] == '\n')
        i++;
    return (i);
}

void    add_export(char **argv, int s, t_env **env)
{
    t_env *current;
    int i;
    char **temp;

    i = 0;
    current = *env;
    while(current->export[i])
        i++;
    temp = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while(current->export[i])
     {
        temp[i] = current->export[i];
        i++;
     }
     temp[i] = cat_dup(argv[s]);
     temp[i + 1] = NULL;
    free(current->export);
    current->export = temp;
    show_export(env);
    return ;
}
void show_export(t_env **env)
{
    int i;
    t_env *current;
    int j;

    i = 0;
    j = 0;
    current = *env;
    while (current->export[i])
        {
            write(1 ,&current->export[i][j], 1);
            j++;
            if (current->export[i][j] == '\0')
            {
                write(1, "\n", 1);
                j = 0;
                i++;
            }
        }
    return ;
}

void show_env(t_env **env)
{
    int i;
    t_env *current;
    int j;

    i = 0;
    j = 0;
    current = *env;
    while (current->env[i])
        {
            write(1 ,&current->env[i][j], 1);
            j++;
            if (current->env[i][j] == '\0')
            {
                write(1, "\n", 1);
                j = 0;
                i++;
            }
        }
    return ;
}

void cd_recognition(char **argv, int i)
{
    char buffer[1024];
    
    i++;
	if (access(argv[i], R_OK) != 0)
		printf("cd: %s: No such file or directory\n", argv[i]);
	if (chdir(argv[i]) != 0)
	return ;
    if (getcwd(buffer, sizeof(buffer)) != NULL)
		printf("%s\n", buffer);
}

void env_recognition(char **argv, t_env **env)
{
	int i;
    t_env *current;

    current = *env;
	i = 0;
	if(!current->env)
		return ;
	while(current->env[i])
	{
		printf("%s\n", current->env[i]);
		i++;
	}
	return ;
}
void pwd_recognition(char **argv, t_env **env)
{
	int i;
	int j;
    t_env *current;
	
	i = 0;
	j = 4;
    current = *env;
    if (!current->env[i])
		return ;
	while (current->env[i] && strncmp(current->env[i],"PWD=",4))
		i++;
	if (current->env[i] && !strncmp(current->env[i],"PWD=", 4))
	{
		while (current->env[i][j] != '\0')
		{
			write(1 ,&current->env[i][j], 1);
			j++;
		}
		printf("\n");
		i++;
	}
	return ;
}

void cmd (char **argv, t_env **env, char **envp)
{
    int i;
    
    i = 1;
    if (ft_strnstr(BUILTIN, argv[i]) == 1)
        Build_in(argv, i, env);
    // else {
    //     Commande(argv, i);
    // }
    return ;
}       

void initialise_env(t_env **env ,char **envp)
{
    t_env *current;
    int i;
    
    i = 0;
    if (!envp)
        return ;
    *env = malloc(sizeof(t_env));
    current = *env;
    while (envp[i])
        i++;
    current->env = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (envp[i])
    {
        current->env[i] = ft_strdup(envp[i]);
        i++;
    }
    current->env[i] = NULL;
    return ;
}

void initialise_exp(t_env **env, char **envp)
{
    t_env *current;
    int i;
    char *temp;
    
    i = 0;
    if (!envp)
        return ;
    current = *env;
    while (envp[i])
        i++;
    current->export = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (envp[i])
    {
        current->export[i] = cat_dup(envp[i]);
        i++;
    }
    current->export[i] = NULL;
    return ;
}
char  *cat_dup(char *s1)
{
   return ft_strjoin("export ", s1);
}

int main (int argc, char **argv, char **envp)
{
    t_env *list;
    t_env **lst;
    
    list = NULL;
    lst = &list;
    if (argc < 2)
        return (0);
    initialise_env(lst, envp);
    initialise_exp(lst, envp);
    // show_export(lst);
    cmd(argv, lst, envp);
    return (0);
}