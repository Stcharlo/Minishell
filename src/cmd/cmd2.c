/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:01:46 by stcharlo          #+#    #+#             */
/*   Updated: 2025/08/13 20:05:20 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
	{
		i++;
		if (str[i] == '\0')
			return (0);
	}
	while (str[i])
	{
		if (!isdigit((unsigned char)str[i]))
			// || (ft_atoi(str) <= ft_atoi("-9223372036854775808"))
			// || (ft_atoi(str) >= ft_atoi("9223372036854775807")))
			return (0);
		i++;
	}
	return (1);
}

void	exit_recognition(char **argv, int i, t_ast **env)
{
	int	a;

	a = 0;
	while (argv[a])
		a++;
	if (a > 2)
	{
		(*env)->env->error_code = 1;
		exit((*env)->env->error_code);
	}
	if (argv[i + 1])
	{
		if (!is_valid_number(argv[i + 1]))
		{
			(*env)->env->error_code = 2;
			valid_number_fail(env, argv[i + 1]);
		}
		else if (argv[i + 1][0] == '-' || argv[i + 1][0] == '+')
		{
			(*env)->env->error_code = (256 + atoi(argv[i + 1])) % 256;
			num_has_sign(env);
		}
		else
		{
			(*env)->env->error_code = atoi(argv[i + 1]) % 256;
			exit((*env)->env->error_code);
		}
	}
	else
	{
		if (g_exit_code >= 128)
		{
			(*env)->env->error_code = g_exit_code;
		}
		exit((*env)->env->error_code);
	}
}

void	num_has_sign(t_ast **env)
{
	exit((*env)->env->error_code);
}

void	valid_number_fail(t_ast **env, char *arg)
{
	fprintf(stderr, "exit: %s: numeric argument required\n", arg);
	exit((*env)->env->error_code);
}

void	echo_recognition(char **argv, int i, t_ast **env)
{
	int	count;

	count = 1;
	i++;
	(*env)->env->error_code = 0;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		count = 0;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (count)
		write(1, "\n", 1);
	return ;
}
