/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tee.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 18:49:49 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/02 18:20:54 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int     count_trunc_append(ASTNode *node)
{
    ASTNode *redir;
    int res;
    int i;

    i = 0;
    res = 0;
    while(i < node->child_count)
    {
        redir = node->children[i];
        while (redir && redir->type == NODE_REDIRECTION)
        {
            if (!strcmp(redir->value, ">") || !strcmp(redir->value, ">>"))
                res++;
            i++;
        }
    }
    return (res);
}

static int	*open_targets(ASTNode *node)
{
    int		*fds;
    int		i;
    int     j;
    int     k;
    ASTNode *redir;
    int fd;
    int redir_count;
    
    redir_count = count_trunc_append(node);
    redir = NULL;
    fds = malloc(redir_count * sizeof(int));
    if (!fds)
    {
        perror("malloc");
        exit(1);
    }
    i = -1;
    k = 0;
    while(i < node->child_count)
    {
        redir = node->children[i];
        while (redir && redir->type == NODE_REDIRECTION)
        {
            // Open and dup2 for each redirection in order
            if (!strcmp(redir->value, ">"))
                fds[k++] = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (!strcmp(redir->value, ">>"))
                fds[k++] = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fds[i] < 0)
            {
                perror(redir->target->value);
                j = 0;
                while (j < k)
                    close(fds[j++]);
                free(fds);
                exit(1);
            }
            i++;
        }
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

void	ft_tee(char **targets, int count)
{
    char	*buffer;
    int		*fds;

    fds = open_targets(targets, count);
    buffer = malloc(1024);
    if (!buffer)
    {
        free(fds);
        return ;
    }
    write_loop(fds, count, buffer);
    cleanup(fds, count, buffer);
}