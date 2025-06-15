/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:46:58 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/15 19:48:05 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define BUILTIN  "echo:pwd:cd:export:unset:env:exit"
# define METACHAR "\t:\n:|:&:;:(:):<:>"

typedef enum type
{
	INVALID = 0,
	WORD,
	PIPE,
	INPUT_REDIRECT,
	OUTPUT_REDIRECT,
	HEREDOC,
	APPEND,
	BUILD_IN,
	END
}					t_type;

typedef enum
{
	NODE_COMMAND,
	NODE_ARGUMENT,
	NODE_REDIRECTION
}					NodeType;

typedef struct s_token
{
	char *value;          // The token string (e.g., "ls", "|", ">", etc.)
	t_type type;          // The type of the token (e.g., WORD, PIPE, etc.)
	struct s_token *next; // Pointer to the next token (for a linked list)
}					t_token;

typedef struct ASTNode
{
	NodeType		type;
	char			*word;
	struct ASTNode	**children;
	int				child_count;
}					ASTNode;

char				**ft_split(const char *s, const char *delim);
t_type				check_type(char *str);
int					is_meta_character(char c);
int					contains_meta_character(char *str);
char				*ft_strjoin_newline(char const *s1, char const *s2);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_token				*ft_lstnew(char *str);
char				*readline_open_quotes(char *str);
int					open_quotes(const char *str);
int					create_list(t_token **start ,char **str);
void				ft_lstadd_back(t_token **lst, t_token *new, char *str);
void				free_stack(t_token **stack);
int					ft_strnstr(const char *big, const char *little);
void				show_list(t_token *list);
int					ft_lstsize(t_token *lst);
#endif