/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:45:55 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 11:40:55 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_exit_code;

static int	get_exit_value(t_ast **env)
{
	if (g_exit_code >= 128)
	{
		(*env)->env->error_code = g_exit_code;
		return (g_exit_code);
	}
	return ((*env)->env->error_code);
}

static void	replace_exit_tokens(t_token *lst, char *exit_str)
{
	while (lst)
	{
		if (!ft_strcmp(lst->value, "$?"))
		{
			free(lst->value);
			lst->value = ft_strdup(exit_str);
		}
		lst = lst->next;
	}
}

void	exit_status(t_token **lst, t_ast **env)
{
	t_token	*tmp;
	char	*exit_str;
	int		exit_value;

	tmp = *lst;
	exit_value = get_exit_value(env) % 256;
	exit_str = ft_itoa(exit_value);
	replace_exit_tokens(tmp, exit_str);
	free(exit_str);
	if (g_exit_code >= 128)
		g_exit_code = 0;
}
