/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:47:21 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/03 06:58:34 by agaroux          ###   ########.fr       */
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
        
        // process_tokens now handles the token list lifecycle fully.
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
    
    if (line == NULL)
        return NULL;
    
    while (open_quotes(line))
    {
        tmp = readline_open_quotes(line);
        if (!tmp) {
            free(line);
            return NULL;
        }
        free(line);
        line = tmp;
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
void	exit_status(t_token **lst, t_ast **env)
{
    t_token	*tmp;
    char	*exit_str;
    int     exit_value;

    tmp = *lst;
    
    // Get the correct exit code: prefer g_exit_code for signals, otherwise use error_code
    if (g_exit_code >= 128) {
        exit_value = g_exit_code;
        // Update the env error_code to match for consistency
        (*env)->env->error_code = g_exit_code;
    } else {
        // Use the environment's error_code for normal command results
        exit_value = (*env)->env->error_code;
    }
    
    // Ensure error code is properly truncated to match Bash behavior
    exit_value = exit_value % 256;
    
    exit_str = ft_itoa(exit_value);
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
    
    // Reset global signal code after using it
    if (g_exit_code >= 128) {
        g_exit_code = 0;
    }
}

/// @brief parsing user input
/// @param lst list for the tokens
/// @param line input from the user
/// @param env 
void	process_tokens(t_token **lst, char *line, t_ast **env)
{
    ASTNode	**nodes;
    t_token_info *tokens;
    int token_count;

    // Apply variable expansion to the line first
    line = unquoted_var_expansion(line, env);
    
    // Use the new quote-aware tokenizer
    tokens = split_bash_style_with_quotes(line, &token_count);
    
    // create_list appends, so we must start with a NULL list.
    *lst = NULL; 
    create_list_with_quote_info(lst, tokens, token_count);

    check_heredoc(lst);
    
    // Check for syntax errors 
    if (check_syntax_errors(*lst))
    {
        (*env)->env->error_code = 2;
        // Free the tokens array
        free(tokens);
        free_stack(lst);
        return;
    }
    
    // Now handle $? expansion specifically in tokens
    exit_status(lst, env); 

    nodes = build_and_print_ast(*lst, env);
    if (nodes && *nodes)
    {
        execute_nodes(nodes, env);
        ast_free(*nodes);
    }
    // Note: Do not reset error_code to 0 when nodes is NULL
    // This preserves the exit code from previous commands or syntax errors

    if (nodes)
        free(nodes);
    unlink_redirection(lst);
    // Free the tokens array
    free(tokens);
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
    
    // Store the final error code before freeing
    int final_exit_code = ASt->env->error_code;
    
    free_env_complete(ASt);
    
    // For signal-related exits (like SIGINT = Ctrl+C)
    if (g_exit_code >= 128)
        return g_exit_code;
        
    // For normal exits, use the shell's stored error code
    // This will be the same as what bash returns with $?
    return (final_exit_code % 256);
}

/**
 * Debug function to print the current exit code
 * Uncomment for debugging only
 */
void print_exit_code(t_ast **env)
{
    printf("Current exit code: %d\n", (*env)->env->error_code);
}
