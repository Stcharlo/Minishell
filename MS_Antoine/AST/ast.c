/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:04:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/18 17:55:04 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Tokenisation/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


ASTNode *create_ast_node(NodeType type, char *value)
{
    ASTNode *new;
    
    new = malloc(sizeof(ASTNode));
    new->type = type;
    new->value = strdup(value);
    new->children = NULL;
    new->child_count = 0;
    return (new);
}

void add_ast_child(ASTNode *parent, ASTNode *child)
{
    parent->child_count++;
    parent->children = realloc(parent->children, sizeof(ASTNode *) * parent->child_count);
    parent->children[parent->child_count - 1] = child;
}

static char	*ft_strjoin_slash(char const *s1, char const *s2)
{
    char	*res;
    char	*tmp;

    tmp = ft_strjoin(s1, "/");
    if (!tmp)
        return (NULL);
    res = ft_strjoin(tmp, s2);
    free(tmp);
    return (res);
}

char	*get_cmd_path(const char *cmd, char **env)
{
    char	*path_var;
    char	**paths;
    char	*full_path;
    int		i;

    if (strchr(cmd, '/'))
        return (strdup(cmd));
    path_var = NULL;
    while (*env)
    {
        if (strncmp(*env, "PATH=", 5) == 0)
        {
            path_var = *env + 5;
            break;
        }
        env++;
    }
    if (!path_var)
        return (NULL);
    paths = ft_split(path_var, ":");
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin_slash(paths[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free(paths);
    return (NULL);
}


NodeType define_type(char *str, char **env)
{
    if (get_cmd_path(str, env))
        return (NODE_COMMAND);
    if (!strcmp(str, "<<") || !strcmp(str, "<") || !strcmp(str, ">>") || !strcmp(str, ">"))
        return (NODE_REDIRECTION);
    return (NODE_ARGUMENT);
}

void ast_print(ASTNode *node, int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
        case NODE_COMMAND:   printf("COMMAND: %s\n", node->value); break;
        case NODE_ARGUMENT:  printf("ARGUMENT: %s\n", node->value); break;
        case NODE_REDIRECTION: printf("REDIRECTION: %s\n", node->value); break;
        case NODE_TARGET: printf("TARGET: %s\n", node->value); break;
        case NODE_PIPE: printf("PIPE: %s\n", node->value); break;
    }
    for (int i = 0; i < node->child_count; i++) {
        ast_print(node->children[i], indent + 1);
    }
}

void count_commands(char **argv, char **env, int *count_cmds, int *count_redirects)
{
    int i;
    
    i = 1;
    *count_cmds = 0;
    *count_redirects = 0;
    while (argv[i])
    {
        if (define_type(argv[i], env) == NODE_COMMAND)
            (*count_cmds)++;
        else if (define_type(argv[i], env) == NODE_REDIRECTION)
            (*count_redirects)++;
        i++;
    }
}

void ast_free(ASTNode *node) {
    if (!node) return;
    free(node->value);
    for (int i = 0; i < node->child_count; i++) {
        ast_free(node->children[i]);
    }
    free(node->children);
    free(node);
}

static void	fill_ast_nodes(ASTNode **head, char **argv, char **env)
{
    int	i;
    int j;
    int k;

    i = 1;
    j = -1;
    while (argv[i])
    {
        if (define_type(argv[i], env) == NODE_COMMAND)
            head[++j] = create_ast_node(NODE_COMMAND, argv[i]);
        else if (define_type(argv[i], env) == NODE_ARGUMENT)
        {
            k = 0;
            while (head[j - k]->type != NODE_COMMAND)
                k++;
            add_ast_child(head[j - k], create_ast_node(NODE_ARGUMENT, argv[i]));
        }
        else if (define_type(argv[i], env) == NODE_REDIRECTION)
        {
            head[++j] = create_ast_node(NODE_REDIRECTION, argv[i++]);
            add_ast_child(head[j], create_ast_node(NODE_TARGET, argv[i]));
        }
        i++;
    }
}

static void	chain_ast_nodes(ASTNode **head, int total)
{
    int	j;

    j = 1;
    while (j < total && head[j])
    {
        add_ast_child(head[j - 1], head[j]);
        j++;
    }
}

void	build_and_print_ast(int argc, char **argv, char **env)
{
    int		count_cmds, count_redirects;
    ASTNode	**head;

    count_commands(argv, env, &count_cmds, &count_redirects);
    head = malloc(sizeof(ASTNode *) * (count_cmds + count_redirects + 1));
    printf("CMDS: %d\nREDI: %d\n", count_cmds, count_redirects);
    fill_ast_nodes(head, argv, env);
    chain_ast_nodes(head, count_cmds + count_redirects);
    ast_print(head[0], 1);
    ast_free(head[0]);
    free(head);
}

int main(int argc, char **argv, char **env)
{
    build_and_print_ast(argc, argv, env);
    return (1);
}
