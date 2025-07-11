/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:45:54 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/08 14:15:30 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void redirection(char **argv, int i)
{
    if (ft_strnstr(BUILTIN, argv[i]))
	{
		if (ft_strnstr("<<", argv[i]))
		if (ft_strnstr(">>", argv[i]))
		if (ft_strnstr(">", argv[i]))
            output_recognition(argv, i);
		if (ft_strnstr("<", argv[i]))
		return ;
	}
	return ;
}
void    output_recognition(char **argv, int i)
{
    int outfile_fd;
    
    i++;
    outfile_fd = open(argv[i], O_RDONLY);
    if (dup2(outfile_fd, STDOUT_FILENO) == -1)
        return ;
}