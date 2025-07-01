/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tee.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/01 19:09:10 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_tee(char **targets, int num_files)
{
    char *buffer;
    int i;
    int j;
    int *fds;

    i = 0;
    if (num_files > 0)
    {
        fds = malloc(num_files * sizeof(int));
        if (!fds)
        {
            perror("malloc");
            exit(1);
        }
    }
    while (i < num_files)
    {
        fds[i] = open(targets[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fds[i] < 0)
        {
            perror(targets[i]);
            j = 0;
            while (j < i)
                close(fds[j++]);
            free(fds);
            exit(1);
        }
        i++;
    }   
}
