/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Precedence_climbing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:18:29 by stcharlo          #+#    #+#             */
/*   Updated: 2025/06/19 16:16:13 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TOUT SA JE VAIS LE CODER PLUS TARD

#include "../includes/minishell.h"

void recognize_builtin(t_token **lst, char **env)
{
	t_token *current;
	
	current = *lst;
	if (ft_strnstr(BUILTIN, current->value))
	{
		if (ft_strnstr("pwd", current->value))
			pwd_recognition(lst, env);
		if (ft_strnstr("env", current->value))
			env_recognition(lst, env);
		if (ft_strnstr("echo", current->value))
			echo_recognition(lst, env);
		if (ft_strnstr("cd", current->value))
			cd_recognition(lst, env);
		if (ft_strnstr("export", current->value))
		if (ft_strnstr("unset", current->value))
		if (ft_strnstr("exit", current->value))
		return ;
	}
	return ;
	//pwd ignore ce qui est ecrit derriere lui et ne prends pas d'option
	//echo prends ce qui est derriere lui jusqua une redirection ou un operateur
	//cd prend un path relatif(a partir du dossier actuelle) ou un path absolu
	//export prends comme echo a la diference est que il lit pas les "" et ''
	//unset sans arguments valide ne fait rien
	//env ne prends rien derriere
	// exit ne prends rien derriere
}

void cd_recognition(t_token **lst, char **envp)
{
	t_token *current;
	char buffer[1024];
	
	current = *lst;
	current = current->next;

	if (getcwd(buffer, sizeof(buffer)) != NULL)
		printf("%s\n", buffer);
	if (!current)
		return ;
	if (access(current->value, R_OK) != 0)
		perror("access");
	if (chdir(current->value) != 0)
		perror("chdir");
	if (getcwd(buffer, sizeof(buffer)) != NULL)
		printf("%s\n", buffer);
	return ;
}
void echo_recognition(t_token **lst, char **envp)
{
	t_token *current;
	
	current = *lst;
	current = current ->next;
	while (current && current->type == 1)
	{
		printf("%s", current->value);
		printf(" ");
		current = current->next;
	}
	printf ("\n");
	return ;
}

void env_recognition(t_token **lst, char **envp)
{
	int i;

	i = 0;
	while(envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return ;
}

void pwd_recognition(t_token **lst, char **envp)
{
	int i;
	int j;
	
	i = 0;
	j = 0;
	while (ft_strnstr(envp[i],"PWD=") != 1)
		i++;
	if (!envp[i])
		return ;
	if (ft_strnstr(envp[i],"PWD=") == 1)
	{
		while(envp[i][j] != '=')
			j++;
		j++;
		while (envp[i][j] != '\0')
		{
			write(1 ,&envp[i][j], 1);
			j++;
		}
		printf("\n");
		i++;
	}
	return ;
}

void priority_check(t_token **lst)
{
	//"<" a besoin d'un input a sa droite
	//">" cree un output si besoin avec le nom a droite
	//"<<" va modifier le fichier et chercher le caractere a droite
	//">>" va faire comme ">" en append mode
}