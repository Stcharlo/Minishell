/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:47:07 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/04 17:24:51 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void Build_in(char **tab, int i, t_ast **env)
{
    if (ft_strnstr(BUILTIN, tab[i]))
	{
		if (ft_strnstr("pwd", tab[i]))
			pwd_recognition(env);
		if (ft_strnstr("env", tab[i]))
		 	env_recognition(env);
		if (ft_strnstr("cd", tab[i]))
            cd_recognition(tab, i);
		if (ft_strnstr("export", tab[i]))
		 	export_recognition(tab, i, env);
		if (ft_strnstr("unset", tab[i]))
            unset_recognition(tab, i, env);
		return ;
	}
	return ;
}

void unset_recognition(char **argv, int i, t_ast **env)
{ 
    t_ast *current;
    
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

void unset_exp(char **argv, int i, t_ast **env)
{
    t_ast *current;
    char    **temp;
    int j;
    int count;

    j = 0;
    count = 0;
    current = *env;
    while (current->env->export[count])
        count++;
    temp = malloc(sizeof(char *) * (count + 1));
    count = 0;
    while (current->env->export[j])
    {
        if (ft_strncmp(current->env->export[j], argv[i], strlen(argv[i])))
            j++;
        temp[count] = current->env->export[j];
        count++;
        j++;
    }
    temp[count] = NULL;
    free(current->env->export);
    current->env->export = temp;
    show_export(env);
    return ;
}

void unset_env(char **argv, int i, t_ast **env)
{
    t_ast *current;
    char    **temp;
    int j;
    int count;

    j = 0;
    count = 0;
    current = *env;
    while (current->env->env[count])
        count++;
    temp = malloc(sizeof(char *) * (count));
    count = 0;
    while (current->env->env[j])
    {
        if (!strncmp(current->env->env[j], argv[i], strlen(argv[i])))
            j++;
        temp[count] = current->env->env[j];
        count++;
        j++;
    }
    temp[count] = NULL;
    free(current->env->env);
    current->env->env = temp;
    show_env(env);
    return ;
}

void export_recognition(char **argv, int i, t_ast **env)
{
    t_ast *current;
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
void    add_env(char **argv, int s, t_ast **env)
{
    t_ast *current;
    int i;
    char **temp;

    i = 0;
    current = *env;
    while(current->env->env[i])
        i++;
    temp = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while(current->env->env[i])
     {
        temp[i] = current->env->env[i];
        i++;
     }
     temp[i] = ft_strdup(argv[s]);
     temp[i + 1] = NULL;
    free(current->env->env);
    current->env->env = temp;
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

void    add_export(char **argv, int s, t_ast **env)
{
    t_ast *current;
    int i;
    char **temp;

    i = 0;
    current = *env;
    while(current->env->export[i])
        i++;
    temp = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while(current->env->export[i])
     {
        temp[i] = current->env->export[i];
        i++;
     }
     temp[i] = cat_dup(argv[s]);
     temp[i + 1] = NULL;
    free(current->env->export);
    current->env->export = temp;
    show_export(env);
    return ;
}
void show_export(t_ast **env)
{
    int i;
    t_ast *current;
    int j;

    i = 0;
    j = 0;
    current = *env;
    while (current->env->export[i])
        {
            write(1 ,&current->env->export[i][j], 1);
            j++;
            if (current->env->export[i][j] == '\0')
            {
                write(1, "\n", 1);
                j = 0;
                i++;
            }
        }
    return ;
}

void show_env(t_ast **env)
{
    int i;
    t_ast *current;
    int j;

    i = 0;
    j = 0;
    current = *env;
    while (current->env->env[i])
        {
            write(1 ,&current->env->env[i][j], 1);
            j++;
            if (current->env->env[i][j] == '\0')
            {
                write(1, "\n", 1);
                j = 0;
                i++;
            }
        }
    return ;
}

void cd_recognition(char **tab, int i)
{
    char buffer[1024];

    i++;
	if (access(tab[i], R_OK) != 0)
		printf("cd: %s: No such file or directory\n", tab[i]);
	if (chdir(tab[i]) != 0)
	return ;
    if (getcwd(buffer, sizeof(buffer)) != NULL)
		printf("%s\n", buffer);
}

void env_recognition(t_ast **env)
{
	int i;
    t_ast *current;

    current = *env;
	i = 0;
	if(!current->env->env)
		return ;
	while(current->env->env[i])
	{
		printf("%s\n", current->env->env[i]);
		i++;
	}
	return ;
}
void pwd_recognition(t_ast **env)
{
	int i;
	int j;
    t_ast *current;
	
	i = 0;
	j = 4;
    current = *env;
    if (!current->env->env[i])
		return ;
	while (current->env->env[i] && strncmp(current->env->env[i],"PWD=",4))
		i++;
	if (current->env->env[i] && !strncmp(current->env->env[i],"PWD=", 4))
	{
		while (current->env->env[i][j] != '\0')
		{
			write(1 ,&current->env->env[i][j], 1);
			j++;
		}
		printf("\n");
		i++;
	}
	return ;
}

int cmd(char **tab, t_ast **env)
{
    int i;
    
    i = 0;
    if (ft_strnstr(BUILTIN, tab[i]) == 1)
    {
        Build_in(tab, i, env);
        return (0);
    }
        // if (ft_strnstr(REDIRECTION, argv[i]) == 1)
    //     Redirection(argv, i, env);
    // else {
    //     Commande(argv, i);
    // }
    return (1);
}
int cmd_recognize(char *tab, t_ast **env)
{
    int i;
    
    i = 0;
    if (ft_strnstr(BUILTIN, tab) == 1)
    {
        return (0);
    }
        // if (ft_strnstr(REDIRECTION, argv[i]) == 1)
    //     Redirection(argv, i, env);
    // else {
    //     Commande(argv, i);
    // }
    return (1);
}             

void initialise_env(t_ast **env ,char **envp)
{
    t_ast *current;
    int i;
    
    i = 0;
    if (!envp)
        return ;
    *env = malloc(sizeof(t_ast));
    (*env)->env = malloc(sizeof(t_env));
    current = *env;
    while (envp[i])
        i++;
    current->env->env= malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (envp[i])
    {
        current->env->env[i] = ft_strdup(envp[i]);
        i++;
    }
    current->env->env[i] = NULL;
    return ;
}

void initialise_exp(t_ast **env, char **envp)
{
    t_ast *current;
    int i;
    char *temp;
    
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
char  *cat_dup(char *s1)
{
   return ft_strjoin("export ", s1);
}

// int main (int argc, char **argv, char **envp)
// {
//     t_env *list;
//     t_env **lst;
//     t_ast *AST;
//     t_ast **ASt;
    
//     AST = NULL;
//     ASt = &AST;
//     list = NULL;
//     lst = &list;
//     if (argc < 2)
//         return (0);
//     initialise_env(ASt, envp);
//     initialise_exp(ASt, envp);
//     // show_export(lst);
//     cmd(argv, ASt, envp);
//     return (0);
// }