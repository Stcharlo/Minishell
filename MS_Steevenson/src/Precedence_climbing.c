/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Precedence_climbing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:18:29 by stcharlo          #+#    #+#             */
/*   Updated: 2025/06/20 15:52:58 by stcharlo         ###   ########.fr       */
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
			//export_recognition(lst, env);
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
	
	if (!current)
		return ;
	current = *lst;
	current = current->next;
	if (!current)
		return ;
	if (access(current->value, R_OK) != 0)
		printf("cd: %s: No such file or directory\n", current->value);
	if (chdir(current->value) != 0)
	return ;
}
void echo_recognition(t_token **lst, char **envp)
{
	t_token *current;
	
	current = *lst;
	current = current ->next;
	if (current && current->type != 1)
		printf("bash: syntax error near unexpected token 'newline'");
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
	if(!envp)
		return ;
	while(envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return ;
}

void pwd_recognition(t_token **lst, char **envp)
{
	char *user;
	
	user = getenv("PWD");
	if (user != NULL)
		printf("%s\n", user);
	else {
		perror("getenv");
	}
}

void priority_check(t_token **lst)
{
	//"<" a besoin d'un input a sa droite
	//">" cree un output si besoin avec le nom a droite
	//"<<" va modifier le fichier et chercher le caractere a droite
	//">>" va faire comme ">" en append mode
}