/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:06:26 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/20 18:08:11 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

int	check_dbl_equal(char *argv)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;

	while (argv[i])
	{
		if (argv[i] == '=')
			temp++;
		i++;
	}
	if (temp > 1)
		return (1);
	return (0);
}

int	skip_isspace(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] == 32 || argv[i] == '\t' || argv[i] == '\n')
		i++;
	return (i);
}

void	add_export(char *argv, t_ast **env)
{
	t_ast	*current;
	int		i;
	char	**temp;

	i = 0;
	current = *env;
	while (current->env->export[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (current->env->export[i])
	{
		temp[i] = current->env->export[i];
		i++;
	}
	temp[i] = cat_dup(argv);
	temp[i + 1] = NULL;
	free(current->env->export);
	current->env->export = temp;
	return ;
}
void	show_export(t_ast **env)
{
	int		i;
	t_ast	*current;
	int		j;

	i = 0;
	j = 0;
	current = *env;
	while (current->env->export[i])
	{
		write(1, &current->env->export[i][j], 1);
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

void	show_env(t_ast **env)
{
	int		i;
	t_ast	*current;
	int		j;

	i = 0;
	j = 0;
	current = *env;
	while (current->env->env[i])
	{
		write(1, &current->env->env[i][j], 1);
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
