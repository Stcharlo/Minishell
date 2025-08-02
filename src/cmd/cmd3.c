/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:37:34 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/02 11:39:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	unset_recognition(char **argv, int i, t_ast **env)
{
	t_ast	*current;

	current = *env;
	i++;
	if (argv[i] == NULL)
	(*env)->env->error_code = 0;
	return ;
	while (argv[i])
	{
		if (search_value(argv[i], env) != 1)
		{
			unset_env(argv[i], env);
			unset_exp(argv[i], env);
		}
		i++;
	}
	return ;
}

int	search_value(char *str, t_ast **env)
{
	t_ast	*current;
	int		j;
	int		count;
	char	*target;

	j = 0;
	count = 0;
	current = *env;
	target = cat_dup(str);
	while (current->env->export[j])
	{
		if (ft_strncmp(current->env->export[j], (target), strlen(target)) != 0)
			j++;
		else
		{
			count++;
			j++;
		}
	}
	free(target);
	if (count > 0) {
		(*env)->env->error_code = 1;
		return (0);
	}
	(*env)->env->error_code = 0;
	return (1);
}

void	unset_exp(char *argv, t_ast **env)
{
	t_ast	*current;
	char	**temp;
	int		j;
	int		count;
	char	*target;

	j = 0;
	current = *env;
	target = cat_dup(argv);
	count = tab_len(current);
	temp = malloc(sizeof(char *) * (count + 1));
	count = 0;
	while (current->env->export[j])
	{
		if (ft_strncmp(current->env->export[j], (target), strlen(target)) != 0)
			temp[count++] = current->env->export[j];
		else if (ft_strncmp(current->env->export[j],
				(target), strlen(target)) == 0)
			free(current->env->export[j]);
		j++;
	}
	temp[count] = NULL;
	free_both(target, current);
	current->env->export = temp;
	return ;
}

int	tab_len(t_ast *current)
{
	int		count;

	count = 0;
	while (current->env->export[count])
		count++;
	return (count);
}

void	free_both(char *target, t_ast *current)
{
	free(target);
	free(current->env->export);
}
