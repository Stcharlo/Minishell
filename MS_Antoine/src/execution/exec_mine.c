/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_mine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:12:41 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/02 17:57:59 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	apply_output_redirections(ASTNode *node)
{
    int		count;
    int		i;
    ASTNode	*redir;

    count = 0;
    i = 0;
    while (i < node->child_count)
    {
        redir = node->children[i];
        if (redir->type == NODE_REDIRECTION &&
            (!ft_strcmp(redir->value, ">") || !ft_strcmp(redir->value, ">>")))
            count++;
        i++;
    }
    if (count == 0)
        return ;
    if (count == 1)
    {
        i = 0;
        while (i < node->child_count)
        {
            redir = node->children[i];
            if (redir->type == NODE_REDIRECTION &&
                (!ft_strcmp(redir->value, ">") || !ft_strcmp(redir->value, ">>")))
            {
                if (!ft_strcmp(redir->value, ">"))
                    dup2(open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644), STDOUT_FILENO);
                else
                    dup2(open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644), STDOUT_FILENO);
                break ;
            }
            i++;
        }
    }
    else
    {
        int		pipefd[2];
        pid_t	pid;

        if (pipe(pipefd) < 0)
        {
            perror("pipe");
            exit(1);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            /* Child process: run tee to duplicate output to all targets */
            close(pipefd[1]);
            {
                char	**targets;
                int		j;

                targets = malloc(sizeof(char *) * (count + 1));
                if (!targets)
                {
                    perror("malloc");
                    exit(1);
                }
                j = 0;
                i = 0;
                while (i < node->child_count)
                {
                    redir = node->children[i];
                    if (redir->type == NODE_REDIRECTION &&
                        (!ft_strcmp(redir->value, ">") || !ft_strcmp(redir->value, ">>")))
                        targets[j++] = redir->target->value;
                    i++;
                }
                targets[j] = NULL;
                ft_tee(targets, count);
            }
            exit(0);
        }
        /* Parent: redirect STDOUT to tee pipe */
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
}

void	exec_cmd(ASTNode *node, char **env)
{
	int		i;
	int		argc;
	char	**tab;

	if (!node)
		return ;
	tab = malloc(sizeof(char *) * (node->child_count + 2));
	tab[0] = node->value;
	i = 0;
	argc = 1;
	while (i < node->child_count)
	{
		if (node->children[i]->type == NODE_ARGUMENT)
			tab[argc++] = node->children[i]->value;
		i++;
	}
	tab[argc] = 0;
	/*if (tab[0])
		printf("%s\n", tab[0]);
	if (tab[1])
		printf("%s\n", tab[1]);*/
	//apply_redirections(node, tab, env);
	apply_output_redirections(node);
	execve(get_cmd_path(tab[0], env), tab, env);
    perror("execve");
	exit(1);
}

void apply_redirections(ASTNode *node, char **tab, char **env)
{
    ASTNode *redir = NULL;
    int fd;

    // Find and apply all redirections in order (left to right)
    for (int i = 0; i < node->child_count; i++)
    {
        redir = node->children[i];
        while (redir && redir->type == NODE_REDIRECTION)
        {
            // Open and dup2 for each redirection in order
            if (!strcmp(redir->value, ">"))
                fd = open(redir->target->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (!strcmp(redir->value, ">>"))
                fd = open(redir->target->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else if (!strcmp(redir->value, "<"))
                fd = open(redir->target->value, O_RDONLY);
            else
                break;
            if (fd < 0)
            {
                perror("open");
                exit(1);
            }
            if (!strcmp(redir->value, "<"))
                dup2(fd, STDIN_FILENO);
            else
                dup2(fd, STDOUT_FILENO);
            close(fd);
            // Move to next nested redirection (if any)
            redir = redir->left;
        }
    }
}

void	execute_nodes(ASTNode **head, char **env)
{
    if (!head || !(*head))
        return;
    exec_ast(*head, env, STDIN_FILENO, STDOUT_FILENO);
}
