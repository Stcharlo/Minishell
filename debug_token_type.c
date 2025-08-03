#include "includes/minishell.h"
#include <stdio.h>

int main()
{
    const char *input = "\"|\"";
    int token_count;
    t_token_info *tokens = split_bash_style_with_quotes(input, &token_count);
    
    printf("Input: %s\n", input);
    printf("Token count: %d\n", token_count);
    
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: '%s' (was_quoted: %d)\n", i, tokens[i].value, tokens[i].was_quoted);
        
        // Test check_type
        int type = check_type(tokens[i].value);
        printf("  check_type result: %d\n", type);
        
        // Test what type would be assigned
        int final_type = tokens[i].was_quoted ? WORD : type;
        printf("  final type: %d (WORD=%d, PIPE=%d)\n", final_type, WORD, PIPE);
    }
    
    free(tokens);
    return 0;
}
