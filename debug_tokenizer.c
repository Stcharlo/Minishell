#include "includes/minishell.h"
#include <stdio.h>

int main()
{
    const char *input = "echo hi |  \"|\"";
    int token_count;
    t_token_info *tokens = split_bash_style_with_quotes(input, &token_count);
    
    printf("Input: %s\n", input);
    printf("Token count: %d\n", token_count);
    
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%s' (was_quoted: %d)\n", i, tokens[i].value, tokens[i].was_quoted);
    }
    
    free(tokens);
    return 0;
}
