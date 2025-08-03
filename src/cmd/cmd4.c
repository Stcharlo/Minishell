/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:59:42 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/03 14:52:59 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	unset_env(char *argv, t_ast **env)
{
	t_ast	*current;
	char	**temp;
	int		j;
	int		count;

	j = 0;
	count = 0;
	current = *env;
	while (current->env->env[count])
		count++;
	temp = malloc(sizeof(char *) * (count));
	count = 0;
	while (current->env->env[j])
	{
		if (ft_strncmp(current->env->env[j], argv, strlen(argv)) != 0)
			temp[count++] = current->env->env[j];
		else if (ft_strncmp(current->env->env[j], argv, strlen(argv)) == 0)
			free(current->env->env[j]);
		j++;
	}
	temp[count] = NULL;
	free(current->env->env);
	current->env->env = temp;
	return ;
}

char	*cat_dup(char *s1)
{
    if (!s1)
        return NULL;
    
    return ft_strjoin("export ", s1);
}

void	export_recognition(char **argv, int i, t_ast **env)
{
	i++;
	if (argv[i] == NULL)
		show_export(env);
	while (argv[i])
	{
		if (parse_exp(argv[i]) == 1)
		{
			(*env)->env->error_code = 1;
			return ;
		}
		if (parse_exp(argv[i]) != 1 && !strchr(argv[i], '=')) {
			add_export(argv[i], env);
			(*env)->env->error_code = 0;
		}
		if (parse_exp(argv[i]) != 1 && strchr(argv[i], '=')) {
			add_env(argv[i], env);
			add_export(argv[i], env);
			(*env)->env->error_code = 0;
		}
		i++;
	}
	(*env)->env->error_code = 0;
	return ;
}

void	add_env(char *argv, t_ast **env)
{
	t_ast	*current;
	int		i;
	char	**temp;

	i = 0;
	current = *env;
	while (current->env->env[i])
		i++;
	temp = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (current->env->env[i])
	{
		temp[i] = current->env->env[i];
		i++;
	}
	temp[i] = ft_strdup(argv);
	temp[i + 1] = NULL;
	free(current->env->env);
	current->env->env = temp;
	return ;
}

int	parse_exp(char *argv)
{
	int	j;

	j = skip_isspace(argv);
	if (argv[j] == '=')
		return (1);
	if (argv[j] >= '0' && argv[j] <= '9')
		return (1);
	if (check_dbl_equal(argv))
		return (1);
	while (isalpha(argv[j]) || isalnum(argv[j]) || argv[j] == '_')
		j++;
	if (argv[j] == '=' || argv[j] == '\0')
		return (0);
	if (argv[j] != '\0' || argv[j] != '=')
		return (1);
	return (1);
}
