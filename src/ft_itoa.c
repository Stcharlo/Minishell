/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:59:31 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/09 16:46:54 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	nb_len(long nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		count++;
		nb *= -1;
	}
	if (nb == 0)
		count++;
	while (nb != 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

static char	*allocate_mem(int len)
{
	char	*m;
	int		i;

	m = malloc((len + 1) * sizeof(char));
	if (!m)
		return (0);
	i = 0;
	while (i <= len)
	{
		m[i] = '0';
		i++;
	}
	return (m);
}

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*result;
	long	nb;

	nb = n;
	len = nb_len(nb);
	result = allocate_mem(len);
	if (!result)
		return (0);
	if (nb < 0)
		nb *= -1;
	i = len - 1;
	while (nb != 0)
	{
		result[i] = ((nb % 10) + 48);
		nb = nb / 10;
		i--;
	}
	if (n < 0)
		*result = '-';
	result[len] = 0;
	return (result);
}
