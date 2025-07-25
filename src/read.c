/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:47:21 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/15 17:24:30 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_exit_code = 0;

int check_type(char *str)
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

ASTNode *create_ast(int type, char *word)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = word;
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
    int i;
    int count_letters;
    int b_meta;
    
    i = 0;
    count_letters = 0;
    b_meta = 0;
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
/// @brief keeps on reading user input, add_history of input, checks for cmds such as clear and exit and calls process_tokens
/// @param lst chained list for the tokenisation
/// @param env 
void	infinite_read(t_token **lst , t_ast **env)
{
    char	*line;

    while (1)
    {
        line = get_input();
        if (!line)
            return ;
        if (line[0] == 0)
            {
                free(line);
                continue;
            }
        add_history(line);
        if (!strcmp(line, "clear"))
            clear_history();
        if (!strcmp(line, "exit"))
        {
            free(line);
            break ;
        }
        process_tokens(lst, line, env);
    }
}
/// @brief reads a complete line of the user input and checks if there are open quotes once quotes are closed it sends the user input
/// @param  
/// @return unparsed input from the user
char	*get_input(void)
{
    char	*line;
    char    *tmp;

    line = readline("Minishell> ");
    while (open_quotes(line))
    {
        tmp = readline_open_quotes(line);
        free(line);
        line = tmp;
        free(tmp);
    }
    return (line);
}

void unlink_redirection(t_token **lst)
{
    t_token *tmp;
    
    tmp = *lst;
    while(tmp)
    {
        if (!ft_strcmp(tmp->value, "<<"))
            unlink(tmp->next->value);
        tmp = tmp->next;
    }
}
void	exit_status(t_token **lst)
{
	t_token	*tmp;
	char	*exit_str;

	tmp = *lst;
	exit_str = ft_itoa(g_exit_code);
	while (tmp)
	{
		if (!ft_strcmp(tmp->value, "$?"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(exit_str);
		}
		tmp = tmp->next;
	}
	free(exit_str);
}

/// @brief parsing user input
/// @param lst list for the tokens
/// @param line input from the user
/// @param env 
void	process_tokens(t_token **lst, char *line, t_ast **env)
{
	ASTNode	**nodes;
	char	**cmd;

	line = unquoted_var_expansion(line, env);
	cmd = split_bash_style(line);
	line = clean_line(line, env);
	create_list(lst, cmd);
	exit_status(lst);
	check_heredoc(lst);
	nodes = build_and_print_ast(*lst, env);
	execute_nodes(nodes, env);
	if (nodes && *nodes)
		ast_free(*nodes);
	unlink_redirection(lst);
    free(nodes);
    free_split(cmd);
    free_stack(lst);   
}

/// @brief main function calling infinite read
/// @param argc 
/// @param argv 
/// @param env 
/// @return 
int	main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    
    t_token *list;
    t_token **lst;
    t_ast **AST;
    t_ast *ASt;
    
    ASt = NULL;
    AST = &ASt;
    list = NULL;
    lst = &list;
    setup_sigint_handler();
	setup_sigquit_handler();
	disable_echoctl();
    initialise_env(AST, env);
    initialise_exp(AST, env);
    initialise_shlvl(AST);
    infinite_read(lst, AST);
    rl_clear_history();
    free_env_complete(ASt);
    return (g_exit_code);
}