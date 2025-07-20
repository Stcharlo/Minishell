/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:01:46 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/20 17:35:34 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_exit_code;

int	is_valid_number(const char *str)
{
	int		i;

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
			return (0);
		i++;
	}
	return (1);
}

void	exit_recognition(char **argv, int i)
{
	int		a;

	a = 0;
	while (argv[a])
		a++;
	if (a > 2)
	{
		g_exit_code = 1;
		exit(g_exit_code);
	}
	if (argv[i + 1])
	{
		if (!is_valid_number(argv[i + 1]))
			valid_number_fail();
		else if (argv[i + 1][0] == '-' || argv[i + 1][0] == '+')
			num_has_sign(argv, i);
		else
		{
			g_exit_code = atoi(argv[i + 1]) % 256;
			exit(g_exit_code);
		}
	}
	else
		exit(g_exit_code);
}

void	num_has_sign(char **argv, int i)
{
	g_exit_code = (256 + atoi(argv[i + 1])) % 256;
	exit(g_exit_code);
}

void	valid_number_fail(void)
{
	strerror(2);
	g_exit_code = 2;
	exit(g_exit_code);
}

void	echo_recognition(char **argv, int i)
{
	int		count;

	count = 1;
	i++;
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
