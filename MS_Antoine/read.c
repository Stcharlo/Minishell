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

//eliminating double quotes
char *double_quotes(char *str, int start, int end)
{
    char *res;
    int index_str;
    int index_res;

    index_str = 0;
    index_res = 0;
    res = malloc(sizeof(char) * strlen(str) + 1);
    if (!res)
        return (NULL);
    while (index_str < start)
        res[index_res++] = str[index_str++];
    while (index_str <= end + 1)
    {
        if (str[index_str] != '\"')
            res[index_res++] = str[index_str++];
        else
            index_str++;
    }
    while (str[index_str])
        res[index_res++] = str[index_str++];
    res[index_res] = 0;
    free(str);
    return (res);
}
//eliminating single_quotes
char *single_quotes(char *str, int start, int end)
{
    char *res;
    int index_str;
    int index_res;

    index_str = 0;
    index_res = 0;
    res = malloc(sizeof(char) * strlen(str) + 1);
    if (!res)
        return (NULL);
    while (index_str < start)
        res[index_res++] = str[index_str++];
    while (index_str <= end + 1)
    {
        if (str[index_str] != '\'')
            res[index_res++] = str[index_str++];
        else
            index_str++;
    }
    while (str[index_str])
        res[index_res++] = str[index_str++];
    res[index_res] = 0;
    free(str);
    return (res);
}

//formatting the lines if their are single of double quotes
char *clean_line(char *str)
{
    int i = 0;
    int start = -1;
    int end = -1;

    while (str[i])
    {
        if (str[i] == '\'')
        {
            start = i;
            while (str[i++] && str[i] != '\'')
                end = i;
            if (str[i] == '\'')
                str = single_quotes(str, start, end);
        }
        else if (str[i] == '\"')
        {
            start = i;
            while (str[i++] && str[i] != '\"')
                end = i;
            if (str[i] == '\"')
                str = double_quotes(str, start, end);
        }
        i++;
    }
    return (str);
}

// check that quotes are still while reading the line
int open_quotes(const char *str)
{
    int s = 0, d = 0, i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && d % 2 == 0)
            s++;
        else if (str[i] == '\"' && s % 2 == 0)
            d++;
        i++;
    }
    printf("s: %d, d: %d, i: %d\n", s, d, i);
    return (s % 2 != 0 || d % 2 != 0);
}

char *readline_open_quotes(char *str)
{
    char *tmp;
    char *res;

    tmp = readline("quote> ");
    if (!tmp)
    {
        free(str); // Free the original string if readline fails
        return (NULL);
    }
    res = ft_strjoin_newline(str, tmp);
    free(str); // Free the original string after joining
    free(tmp); // Free the temporary string
    if (!res)
        return (NULL);
    return (res);
}

void infinite_read(void)
{
    int i;
    char *line;
    char **cmd;
    
    while (1)
    {
        line = readline("Minishell> ");
        while (open_quotes(line))
            line = readline_open_quotes(line);
        if (!line)
            continue;
        printf("%s\n", line);
        add_history(line);
        if (!strcmp(line, "clear"))
            clear_history();
        //when signal ctrl + C
        //rl_on_new_line();
        if (!strcmp(line, "exit"))
            break;
        cmd = ft_split(line, "\t\n|&;()<>");
        i = 0;
        while (cmd[i])
        {
            //clean_line(cmd[i]);
            printf("Token: %s, Type: %d\n", cmd[i], check_type(cmd[i]));
            i++;
        }
    }
}

int main()
{
    char *line;

    infinite_read();
    return (0);
}