/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:45:54 by stcharlo          #+#    #+#             */
/*   Updated: 2025/07/03 16:02:46 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void Redirection(char **argv, int i, t_env **env)
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