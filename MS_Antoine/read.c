/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:47:21 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/17 16:53:39 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

/*
Control operators are:

newline, |, ||, &, &&, ;, ;;, ;&, ;;&, |&, (, )

Redirection operators are:

<, >, <<, >>, <&, >|, <←, <>, >&
*/

t_type check_type(char *str)
{
    if (!strcmp(str, "|"))
        return (PIPE);
    else if (!strcmp(str, "<"))
        return(INPUT_REDIRECT);
    else if (!strcmp(str, "<<"))
        return(HEREDOC);
    else if (!strcmp(str, ">"))
        return(OUTPUT_REDIRECT);
    else if (!strcmp(str, ">>"))
        return(APPEND);
    else if (contains_meta_character(str))
        return(INVALID);
    else
        return (WORD);
    // Make the difference between command and args ? Need to check access_ok => command otherwise just args   
}

ASTNode *create_ast(NodeType type, char *word)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->word = word;
    node->children = NULL;
    node->child_count = 0;
    return (node);
}

int contains_meta_character(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (is_meta_character(str[i]))
            return (1);
            i++;
    }
    return (0);
}
int is_meta_character(char c)
{
    int i = 0;
    char *str = " \t\n|&;()<>";
    while (str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

void separate_tokens(char *str)
{
    int i = 0;
    int count_letters;
    int word = 0;
    int b_meta = 0;

    while (str[i])
    {
        while (isspace(str[i]))
            i++;
        while(is_meta_character(str[i]))
        {
            b_meta = 1;
            count_letters++;
            i++;
        }
        while (str[i] && !isspace(str[i]) && !is_meta_character(str[i]))
        {
            count_letters++;
            i++;
        }
    }
}

void	infinite_read(char **env)
{
    char	*line;

    while (1)
    {
        line = get_input();
        if (!line)
            continue;
        add_history(line);
        if (!strcmp(line, "clear"))
            clear_history();
        if (!strcmp(line, "exit"))
        {
            free(line);
            break ;
        }
        process_tokens(line, env);
    }
}

static char	*get_input(void)
{
    char	*line;
    char    *tmp;

    line = readline("Minishell> ");
    while (open_quotes(line))
    {
        tmp = readline_open_quotes(line);
        free(line);
        line = tmp;
    }
    return (line);
}

static void	process_tokens(char *line, char **env)
{
    char	**cmd;
    int		i;

    line = unquoted_var_expansion(line, env);
    line = clean_line(line, env);
    cmd = ft_split(line, "\t\n|&;()<>");
    i = 0;
    while (cmd && cmd[i])
    {
        printf("Token: %s, Type: %d\n", cmd[i], check_type(cmd[i]));
        i++;
    }
}

int	main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    infinite_read(env);
    return (0);
}