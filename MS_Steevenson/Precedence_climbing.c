/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Precedence_climbing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:18:29 by stcharlo          #+#    #+#             */
/*   Updated: 2025/06/15 20:19:13 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TOUT SA JE VAIS LE CODER PLUS TARD

#include "minishell.h"

void recognize_builtin(t_token **lst)
{
	//pwd ignore ce qui est ecrit derriere lui et ne prends pas d'option
	//echo prends ce qui est derriere lui jusqua une redirection ou un operateur
	//cd prend un path relatif(a partir du dossier actuelle) ou un path absolu
	//export prends comme echo a la diference est que il lit pas les "" et ''
	//unset sans arguments valide ne fait rien
	//env ne prends rien derriere
	// exit ne prends rien derriere
}

void priority_check(t_token **lst)
{
	//"<" a besoin d'un input a sa droite
	//">" cree un output si besoin avec le nom a droite
	//"<<" va modifier le fichier et chercher le caractere a droite
	//">>" va faire comme ">" en append mode
}