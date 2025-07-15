/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:04:04 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/13 14:44:24 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


ASTNode *create_ast_node(int type, char *value)
{
    ASTNode *new;
    
    new = malloc(sizeof(ASTNode));
    if (!new)
        return (NULL);
    new->type = type;
    new->value = strdup(value);
    if (!new->value)
        return (NULL);
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;
    new->children  = NULL;
    new->child_count = 0;
    new->target = NULL;
    return (new);
}

void add_ast_child(ASTNode *parent, ASTNode *child)
{
    ASTNode **new_children;
    int i;

    if (!parent || !child)
        return;
    new_children = malloc(sizeof(ASTNode *) * (parent->child_count + 1));
    if (!new_children)
        return;
    for (i = 0; i < parent->child_count; i++)
        new_children[i] = parent->children[i];
    new_children[parent->child_count] = child;
    free(parent->children);
    parent->children = new_children;
    parent->child_count++;
    child->parent = parent;
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

char	*get_cmd_path(const char *cmd, t_ast **env)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	int		i;
    t_ast   *current;
    
    current = *env;
	if (strchr(cmd, '/'))
		return (strdup(cmd));
	path_var = NULL;
    i = 0;
	while (current->env->env[i])
	{
		if (strncmp(current->env->env[i], "PATH=", 5) == 0)
		{
			path_var = current->env->env[i] + 5;
			break;
		}
		i++;
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
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}


int define_type(char *str, t_ast **env)
{
	char *cmd_path;

	cmd_path = get_cmd_path(str, env);
	if (cmd_path)
	{
		free(cmd_path);
		return (NODE_COMMAND);
	}
	if (!strcmp(str, "<<") || !strcmp(str, "<") || !strcmp(str, ">>") || !strcmp(str, ">"))
		return (NODE_REDIRECTION);
	if (!strcmp(str,"|"))
		return (NODE_PIPE);
	return (NODE_ARGUMENT);
}

void ast_free(ASTNode *node) {
    if (!node) return;
    free(node->value);
    for (int i = 0; i < node->child_count; i++) {
        ast_free(node->children[i]);
    }
    if (node->target)
        ast_free(node->target);
    free(node->children);
    free(node);
}

void set_ast_left(ASTNode *parent, ASTNode *child)
{
    parent->left = child;
    if (child)
        child->parent = parent;
}

void set_ast_right(ASTNode *parent, ASTNode *child)
{
    parent->right = child;
    if (child)
        child->parent = parent;
}
void add_redirection(ASTNode *cmd, ASTNode *redir)
{
    add_ast_child(cmd, redir);
}
void set_redirection_target(ASTNode *redir, ASTNode *target)
{
    redir->target = target;
}

// Remove left chaining, add all redirs as children
ASTNode *parse_command(t_token **lst_ptr, t_ast **env)
{
    t_token *lst = *lst_ptr;
    ASTNode *cmd = NULL;

    // 1. Find the command
    while (lst && define_type(lst->value, env) != NODE_PIPE)
    {
        if (define_type(lst->value, env) != NODE_REDIRECTION)
        {
            cmd = create_ast_node(NODE_COMMAND, lst->value);
            lst = lst->next;
            break;
        }
        // Skip redirection and its target
        lst = lst->next;
        if (lst)
            lst = lst->next;
    }

    // 2. Add arguments (skip redirections and pipes)
    while (lst && define_type(lst->value, env) != NODE_PIPE)
    {
        if (define_type(lst->value, env) != NODE_REDIRECTION)
        {
            add_ast_child(cmd, create_ast_node(NODE_ARGUMENT, lst->value));
            lst = lst->next;
        }
        else
        {
            // Skip redirection and its target
            lst = lst->next;
            if (lst)
                lst = lst->next;
        }
    }

    *lst_ptr = lst;
    return cmd;
}

void attach_redirections(ASTNode *cmd, t_token *lst, t_ast **env)
{
    t_token *cur = lst;
    while (cur && define_type(cur->value, env) != NODE_PIPE)
    {
        if (define_type(cur->value, env) == NODE_REDIRECTION)
        {
            t_token *redir_token = cur;
            ASTNode *redir_node = create_ast_node(NODE_REDIRECTION, redir_token->value);

            // Move to target token
            cur = cur->next;
            ASTNode *target_node = NULL;
            if (cur && define_type(cur->value, env) != NODE_PIPE && define_type(cur->value, env) != NODE_REDIRECTION)
                target_node = create_ast_node(NODE_TARGET, cur->value);

            set_redirection_target(redir_node, target_node);
            add_ast_child(cmd, redir_node);
        }
        cur = cur->next;
    }
}

ASTNode	*parse_pipeline(t_token **lst_ptr, t_ast **env)
{
    ASTNode	*left;
    ASTNode	*pipe;
    ASTNode	*right;
    t_token	*lst = *lst_ptr; // Save the original pointer

    left = parse_command(lst_ptr, env); // advances lst_ptr
    attach_redirections(left, lst, env); // use the original pointer!
    t_token *cur = *lst_ptr;
    if (cur && define_type(cur->value, env) == NODE_PIPE)
    {
        pipe = create_ast_node(NODE_PIPE, cur->value);
        set_ast_left(pipe, left);
        *lst_ptr = cur->next;
        right = parse_pipeline(lst_ptr, env);
        set_ast_right(pipe, right);
        return (pipe);
    }
    return (left);
}

ASTNode  **combine (ASTNode **head, ASTNode *cmd)
{
    ASTNode **new_head;
    
    if (!head) {
        head = malloc(sizeof(ASTNode *));
        *head = NULL;
    }
    if (!*head)
    {
        *head = cmd;
        return (head);
    }
    else
    {
        new_head = malloc(sizeof(ASTNode *));
        *new_head = create_ast_node(NODE_PIPE, "|");
        set_ast_left(*new_head, *head);
        set_ast_right(*new_head, cmd);
        return (new_head);
    }
    
}

static const char *ast_type_str(int type)
{
    if (type == NODE_COMMAND) return "COMMAND";
    if (type == NODE_ARGUMENT) return "ARGUMENT";
    if (type == NODE_REDIRECTION) return "REDIRECTION";
    if (type == NODE_TARGET) return "TARGET";
    if (type == NODE_PIPE) return "PIPE";
    return "UNKNOWN";
}

void	ast_print(ASTNode *node, int indent)
{
    int	i;

    if (!node)
        return;
    for (i = 0; i < indent; i++)
        printf("  ");
    printf("%s: %s\n", ast_type_str(node->type), node->value ? node->value : "(null)");

    // Print children (for commands)
    if (node->child_count > 0)
    {
        for (i = 0; i < indent + 1; i++)
            printf("  ");
        printf("Children:\n");
        for (i = 0; i < node->child_count; i++)
            ast_print(node->children[i], indent + 2);
    }
    // Print left/right (for pipes)
    if (node->left)
    {
        for (i = 0; i < indent + 1; i++)
            printf("  ");
        printf("Left:\n");
        ast_print(node->left, indent + 2);
    }
    if (node->right)
    {
        for (i = 0; i < indent + 1; i++)
            printf("  ");
        printf("Right:\n");
        ast_print(node->right, indent + 2);
    }
    // Print target (for redirections)
    if (node->target)
    {
        for (i = 0; i < indent + 1; i++)
            printf("  ");
        printf("Target:\n");
        ast_print(node->target, indent + 2);
    }
}

void	free_split(char **split)
{
	int i = 0;
	if (!split)
		return;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

ASTNode 	**build_and_print_ast(t_token *lst, t_ast **env)
{
    ASTNode  **root;

    root = malloc(sizeof(ASTNode *));
    *root = parse_pipeline(&lst, env);
    //if (*root)
    //    ast_print(*root, 0);
    return (root);
}
