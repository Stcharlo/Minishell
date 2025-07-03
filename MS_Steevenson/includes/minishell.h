/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:46:58 by agaroux           #+#    #+#             */
/*   Updated: 2025/06/29 18:43:55 by stcharlo         ###   ########.fr       */
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
# include <dirent.h>
# include <fcntl.h>

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
	END
}					t_type;

typedef enum
{
	NODE_COMMAND,
	NODE_ARGUMENT,
	NODE_REDIRECTION,
	NODE_TARGET,
	NODE_PIPE,
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
	char			*value;
	struct ASTNode	**children;
	int				child_count;
}					ASTNode;

// Pour les test

typedef struct s_env
{
	char **env;
	char **export;
}	t_env;

char				**ft_split(const char *s, const char *delim);
char				*ft_strdup(const char *s1);
size_t				ft_strlen(char const *src);
t_type				check_type(char *str);
int					is_meta_character(char c);
int					contains_meta_character(char *str);
char				*ft_strjoin_buffer(char const *s1, char const *s2,
						char buffer);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);

char				*readline_open_quotes(char *str);
int					open_quotes(const char *str);
char				*clean_line(char *str, char **env);
char				**ft_split_once_range(const char *s, char sep, int start,
						int end);
char				**ft_split_dollar_range(const char *s, int start, int end);

char				*ft_substr(char const *s, unsigned int start, size_t len);
void				*ft_calloc(size_t nmemb, size_t size);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strchr(const char *s, int c);
int					ft_strnstr(char *big, char *little);

static void			process_tokens(t_token **lst, char *line, char **env);
static char			*get_input(void);
void				infinite_read(t_token **lst, char **env);
char				*get_value(char *var, int n, char **env);

char				*unquoted_var_expansion(char *str, char **env);
char				*expand_unquoted_var_at(char *str, int start, int len,
						char **env);
char				*expand_variable(char *str, char **env);
static int			find_next_expand(const char *str, int *start, int *len);
static char			*expand_one(const char *str, int start, int len,
						char **env);
						
int					ft_strnstr(char *big, char *little);
void				show_list(t_token *list);
void				free_stack(t_token **stack);
int					ft_lstsize(t_token *lst);
void				ft_lstadd_back(t_token **lst, t_token *new, char *str);
int					create_list(t_token **start ,char **str);
t_token				*ft_lstnew(char *str);

// Pour les test
void				recognize_builtin(char **argv, char **env);
void				pwd_recognition(char **argv, t_env **env);
void				env_recognition(char **argv, t_env **env);
void				echo_recognition(char **argv, t_env **env);
void				cd_recognition(char **argv, int i);
void 				Build_in(char **argv, int i, t_env **env);
void 				export_recognition(char **argv, int i, t_env **env);
void				add_export(char **argv, int s, t_env **env);
void				show_env(t_env **env);
void				unset_env(char **argv, int i, t_env **env);
void				unset_recognition(char **argv, int i, t_env **env);
void 				show_export(t_env **env);
char				*cat_dup(char *s1);
int					parse_exp(char *argv);
int					check_dbl_equal(char *argv);
int					skip_isspace(char *argv);
void				add_env(char **argv, int s, t_env **env);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				unset_exp(char **argv, int i, t_env **env);
#endif