/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:46:58 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/03 16:46:32 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define BUILTIN "echo:pwd:cd:export:unset:env:exit"
# define METACHAR "\t:\n:|:&:;:(:):<:>"
# define REDIRECTION "<:>:>>:<<"

# define INVALID 0
# define COMMAND 1
# define PIPE 2
# define INPUT_REDIRECT 3
# define OUTPUT_REDIRECT 4
# define HEREDOC 5
# define APPEND 6
# define WORD 7
# define END 8

# define NODE_COMMAND 0
# define NODE_ARGUMENT 1
# define NODE_REDIRECTION 2
# define NODE_TARGET 3
# define NODE_PIPE 4

# define LEFT 0
# define RIGHT 1
typedef struct s_token
{
	char				*value;
	int					type;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_env
{
	char **env;
	char **export;
}	t_env;


typedef struct s_ast	ASTNode;

typedef struct					s_ast
{
	int					type;
	char *value; // e.g. "echo", ">", "|", etc.

	// For pipeline and binary operators:
	ASTNode *left;  // Left child (e.g. left command or left pipe)
	ASTNode *right; // Right child (e.g. right command or right pipe)

	// For commands:
	ASTNode **children; // Array of pointers to arguments and redirections
	int					child_count;

	// For redirections:
	ASTNode *target; // For redirection nodes, points to the target file node

	// Optionally:
	ASTNode *parent; // Pointer to parent node (can be NULL for root)
	t_env *env;
} t_ast;

// ast
ASTNode					*create_ast_node(int type, char *value);
void					add_ast_child(ASTNode *parent, ASTNode *child);
static char				*ft_strjoin_slash(char const *s1, char const *s2);
char					*get_cmd_path(const char *cmd, char **env);
int						define_type(char *str, char **env);
void					ast_print(ASTNode *node, int indent);
void					count_commands(t_token *lst, char **env,
							int *count_heads);
void					ast_free(ASTNode *node);
static void				fill_ast_nodes(ASTNode **head, t_token *lst,
							char **env);
static void				chain_ast_nodes(ASTNode **head, int total);
void					push_node_on_top(ASTNode **head, int a);
void					rotate_nodes(ASTNode **head, int a, int b);
void					reorder_ast_nodes(ASTNode **head, int total);
ASTNode					**build_and_print_ast(t_token *lst, char **env);

char					**ft_split(const char *s, const char *delim);
char					*ft_strdup(const char *s1);
size_t					ft_strlen(char const *src);
int 					ft_strcmp(char *s1, char *s2);
int						check_type(char *str);
int						is_meta_character(char c);
int						contains_meta_character(char *str);
char					*ft_strjoin_buffer(char const *s1, char const *s2,
							char buffer);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);

char					*readline_open_quotes(char *str);
int						open_quotes(const char *str);
char					*clean_line(char *str, char **env);
char					**ft_split_once_range(const char *s, char sep,
							int start, int end);
char					**ft_split_dollar_range(const char *s, int start,
							int end);

char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					*ft_calloc(size_t nmemb, size_t size);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strchr(const char *s, int c);

static void				process_tokens(t_token **lst, char *line, char **env);
static char				*get_input(void);
void					infinite_read(t_token **lst, char **env);
char					*get_value(char *var, int n, char **env);

char					*unquoted_var_expansion(char *str, char **env);
char					*expand_unquoted_var_at(char *str, int start, int len,
							char **env);
char					*expand_variable(char *str, char **env);
static int				find_next_expand(const char *str, int *start, int *len);
static char				*expand_one(const char *str, int start, int len,
							char **env);

int						ft_strnstr(char *big, char *little);
void					show_list(t_token *list);
void					free_stack(t_token **stack);
int						ft_lstsize(t_token *lst);
void					ft_lstadd_back(t_token **lst, t_token *new, char *str);
int						create_list(t_token **start, char **str);
t_token					*ft_lstnew(char *str);


void					execute_nodes(ASTNode **head, char **env);

void					exec_pipe_right(ASTNode *node, char **env,
							int output_fd, int *fd);
void					exec_pipe_left(ASTNode *node, char **env, int input_fd,
							int *fd);
void					exec_command_node(ASTNode *node, char **env,
							int input_fd, int output_fd);
void					exec_pipe_node(ASTNode *node, char **env, int input_fd,
							int output_fd);
void					exec_ast(ASTNode *node, char **env, int input_fd,
							int output_fd);
void					exec_cmd(ASTNode *node, char **env);
void					apply_redirections(ASTNode *node, char **tab, char **en);


// Pour les test
void				recognize_builtin(char **argv, char **env);
void				pwd_recognition(char **argv, t_ast **env);
void				env_recognition(char **argv, t_ast **env);
void				echo_recognition(char **argv, t_ast **env);
void				cd_recognition(char **argv, int i);
void 				Build_in(char **argv, int i, t_ast **env);
void 				export_recognition(char **argv, int i, t_ast **env);
void				add_export(char **argv, int s, t_ast **env);
void				show_env(t_ast **env);
void				unset_env(char **argv, int i, t_ast **env);
void				unset_recognition(char **argv, int i, t_ast **env);
void 				show_export(t_ast **env);
char				*cat_dup(char *s1);
int					parse_exp(char *argv);
int					check_dbl_equal(char *argv);
int					skip_isspace(char *argv);
void				add_env(char **argv, int s, t_ast **env);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				unset_exp(char **argv, int i, t_ast **env);
void				Redirection(char **argv, int i, t_env **env);
void				output_recognition(char **argv, int i);

//tee
int     count_trunc_append(ASTNode *node);
static int	*open_targets(ASTNode *node);
static void	cleanup(int *fds, int num_files, char *buffer);
static void	write_loop(int *fds, int num_files, char *buffer);
void	ft_tee(char **targets, int count);
#endif