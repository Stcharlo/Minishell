/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tee.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/03 13:47:47 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	*open_targets(char **targets, int *append_flags, int count)
{
    int		*fds;
    int		i;

    fds = malloc(count * sizeof(int));
    if (!fds)
    {
        perror("malloc");
        exit(1);
    }
    i = 0;
    while (i < count)
    {
        if (append_flags[i])
            fds[i] = open(targets[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fds[i] = open(targets[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fds[i] < 0)
        {
            perror(targets[i]);
            int j = 0;
            while (j < i)
                close(fds[j++]);
            free(fds);
            exit(1);
        }
        i++;
    }
    return (fds);
}

static void	cleanup(int *fds, int num_files, char *buffer)
{
    int	i;

    i = 0;
    while (i < num_files)
        close(fds[i++]);
    free(fds);
    free(buffer);
}

static void	write_loop(int *fds, int num_files, char *buffer)
{
    ssize_t	bytes_read;
    int		i;
    int		j;

    while ((bytes_read = read(STDIN_FILENO, buffer, 1024)) > 0)
    {
        i = 0;
        while (i < num_files)
        {
            if (write(fds[i], buffer, bytes_read) < 0)
            {
                perror("write");
                j = 0;
                while (j < num_files)
                    close(fds[j++]);
                free(fds);
                free(buffer);
                exit(1);
            }
            i++;
        }
    }
}

void	ft_tee(char **targets, int *append_flags, int count)
{
    char	*buffer;
    int		*fds;

    fds = open_targets(targets, append_flags, count);
    buffer = malloc(1024);
    if (!buffer)
    {
        free(fds);
        return ;
    }
    printf("Tee targets: ");
    for (int k = 0; k < count; k++)
        printf("%s (%s) ", targets[k], append_flags[k] ? ">>" : ">");
    printf("\n");
    write_loop(fds, count, buffer);
    cleanup(fds, count, buffer);
}