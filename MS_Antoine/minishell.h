/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:46:58 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/12 18:03:00 by agaroux          ###   ########.fr       */
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

typedef enum type
{
	INVALID = 0,
	WORD,
	PIPE,
	INPUT_REDIRECT,
	OUTPUT_REDIRECT,
	HEREDOC,
	APPEND,
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

char				*readline_open_quotes(char *str);
int					open_quotes(const char *str);

#endif