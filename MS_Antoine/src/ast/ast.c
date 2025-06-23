/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:04:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/23 11:35:00 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
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
    if (!strcmp(str,"|"))
        return (NODE_PIPE);
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

void count_commands(t_token *lst, char **env, int *count_heads)
{
    *count_heads = 0;
    while (lst)
    {
        if (define_type(lst->value, env) == NODE_COMMAND)
            (*count_heads)++;
        else if (define_type(lst->value, env) == NODE_REDIRECTION)
            (*count_heads)++;
        else if (define_type(lst->value, env) == NODE_PIPE)
            (*count_heads)++;
        lst = lst->next;
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

static void	fill_ast_nodes(ASTNode **head, t_token *lst, char **env)
{
    int j;
    int k;

    j = -1;
    while (lst)
    {
        if (define_type(lst->value, env) == NODE_COMMAND)
            head[++j] = create_ast_node(NODE_COMMAND, lst->value);
        else if (define_type(lst->value, env) == NODE_ARGUMENT)
        {
            k = 0;
            while (head[j - k]->type != NODE_COMMAND && head[j - k])
                k++;
            add_ast_child(head[j - k], create_ast_node(NODE_ARGUMENT, lst->value));
        }
        else if (define_type(lst->value, env) == NODE_PIPE)
            head[++j] = create_ast_node(NODE_PIPE, lst->value);
        else if (define_type(lst->value, env) == NODE_REDIRECTION)
        {
            head[++j] = create_ast_node(NODE_REDIRECTION, lst->value);
            lst = lst->next;
            add_ast_child(head[j], create_ast_node(NODE_TARGET, lst->value));
        }
        lst = lst->next;
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

void push_node_on_top(ASTNode **head, int a)
{
    ASTNode *tmp;
    int i;

    if (a <= 0)
        return;
    tmp = head[a];
    i = a;
    while (i > 0)
    {
        head[i] = head[i - 1];
        i--;
    }
    head[0] = tmp;
}

void rotate_nodes(ASTNode **head, int a, int b)
{
    ASTNode *tmp;
    tmp = head[a];
    head[a] = head[b];
    head[b] = tmp;
}

void reorder_ast_nodes(ASTNode **head, int total)
{
    int i;

    i = 0;
    while (i < total)
    {
        if (head[i] && head[i]->type == NODE_REDIRECTION)
        {
                push_node_on_top(head, i);
        }
        i++;
    }
    i = 0;
    while (i < total)
    {
        if (head[i] && head[i]->type == NODE_PIPE)
        {
                rotate_nodes(head, i-1, i);
        }
        i++;
    } 
}

ASTNode 	**build_and_print_ast(t_token *lst, char **env)
{
    int		count_heads;
    ASTNode	**head;

    count_commands(lst, env, &count_heads);
    head = malloc(sizeof(ASTNode *) * (count_heads));
    printf("Count heads: %d\n", count_heads);
    fill_ast_nodes(head, lst, env);
    reorder_ast_nodes(head, count_heads);
    chain_ast_nodes(head, count_heads);
    ast_print(head[0], 1);
    ast_free(head[0]);
    return (head);
}
