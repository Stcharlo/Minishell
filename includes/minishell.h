/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:46:58 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 13:04:14 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L
# define ECHOCTL 0001000

// Global variables
extern int				g_exit_code;

# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define BUILTIN "echo:pwd:cd:export:unset:env:exit"
# define METACHAR "\t:\n:|:&:;:(:):<:>"
# define REDIRECTION "|:<:>:<<:>>"

# define PARENT 0
# define CHILD 1

# define BUFFER_SIZE 1024
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

typedef struct s_pipe_data
{
	int					*fd;
	pid_t				*pids;
	int					input_fd;
	int					output_fd;
}						t_pipe_data;

typedef struct s_token_info
{
	char				*value;
	int					was_quoted;
}						t_token_info;

typedef struct s_token
{
	char				*value;
	int					type;
	int					was_quoted;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_env
{
	char				**env;
	char				**export;
	int					error_code;
	pid_t				last_pid;
}						t_env;

typedef struct s_extract_state
{
	const char			*p;
	char				*token;
	int					i;
	int					in_single_quote;
	int					in_double_quote;
	t_token_info		info;
}						t_extract_state;

typedef struct s_ast	ASTNode;

typedef struct s_ast
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
	t_env				*env;
}						t_ast;

// ast
ASTNode					*create_ast_node(int type, char *value);
void					add_ast_child(ASTNode *parent, ASTNode *child);
char					*get_cmd_path(const char *cmd, t_ast **env);
int						define_type(char *str, t_ast **env);
void					ast_print(ASTNode *node, int indent);
void					ast_free(ASTNode *node);
void					set_ast_left(ASTNode *parent, ASTNode *child);
void					set_ast_right(ASTNode *parent, ASTNode *child);
void					add_redirection(ASTNode *cmd, ASTNode *redir);
void					set_redirection_target(ASTNode *redir, ASTNode *target);
ASTNode					*parse_command(t_token **lst_ptr, t_ast **env);
ASTNode					*parse_pipeline(t_token **lst_ptr, t_ast **env);
ASTNode					**combine(ASTNode **head, ASTNode *cmd);
void					free_split(char **split);
void					handle_backslash(t_extract_state *state);
void					handle_quote(t_extract_state *state, char quote_type);
ASTNode					**build_and_print_ast(t_token *lst, t_ast **env);
void					handle_special_chars(t_extract_state *state);
int						should_break(t_extract_state *state);
void					init_extract_state(t_extract_state *state,
							const char *p);
char					**ft_split(char *s, const char *delim);
/* Functions from ft_split_utils.c */
int						is_delimiter(char c, const char *delim);
int						skip_spaces(const char *s, int i);
int						segment_length(const char *s, const char *delim, int i);
int						count_words(const char *s, const char *delim);
void					free_all(char **psplit);
/* End of ft_split_utils.c functions */
char					*ft_strdup(const char *s1);
size_t					ft_strlen(char const *src);
int						ft_strcmp(char *s1, char *s2);
int						check_type(char *str);
int						is_meta_character(char c);
int						contains_meta_character(char *str);
char					*ft_strjoin_buffer(char const *s1, char const *s2,
							char buffer);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);
int						process_redirection_child(ASTNode *child);

char					*readline_open_quotes(char *str);
int						open_quotes(const char *str);
char					*clean_line(char *str, t_ast **env);
char					**ft_split_once_range(const char *s, char sep,
							int start, int end);
char					**ft_split_dollar_range(const char *s, int start,
							int end);
void					tab_to_file(char **lines, const char *filename);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					*ft_calloc(size_t nmemb, size_t size);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strchr(const char *s, int c);
int						open_tempfile(const char *filename, char *temp_path,
							size_t path_size);
void					process_tokens(t_token **lst, char *line, t_ast **env);
char					*get_input(void);
void					infinite_read(t_token **lst, t_ast **env);
char					*get_value(char *var, int n, t_ast **env);
int						write_all_lines(int fd, char **lines);
void					link_or_copy_temp(const char *temp_path,
							const char *filename);
char					*unquoted_var_expansion(char *str, t_ast **env);
char					*expand_unquoted_var_at(char *str, int start, int len,
							t_ast **env);
char					*expand_variable(char *str, t_ast **env);
int						find_next_expand(const char *str, int *start, int *len);
char					*expand_one(const char *str, int start, int len,
							t_ast **env);
int						is_limiter_line(char *line, char *limiter);
char					*ft_strjoin_slash(char const *s1, char const *s2);
int						is_directory(const char *path);
int						is_var_char(char c);
int						is_var_char2(char c);
void					handle_quotes(const char *str, int i,
							int *in_single_quotes, int *in_double_quotes);
int						handle_var_expansion(char **str, int *i, t_ast **env,
							int in_single_quotes);
int						ft_strnstr(char *big, char *little);
int						check_redirection_without_file(t_token *lst);
int						check_invalid_combinations(t_token *lst);
void					show_list(t_token *list);
void					free_stack(t_token **stack);
int						ft_lstsize(t_token *lst);
void					ft_lstadd_back(t_token **lst, t_token *new, char *str);
void					ft_lstadd_back_with_quote_info(t_token **lst,
							t_token *new, char *str, int was_quoted);
int						create_list(t_token **start, char **str);
int						create_list_with_quote_info(t_token **start,
							t_token_info *tokens, int token_count);
t_token					*ft_lstnew(char *str);
t_token					*ft_lstnew_with_quote_info(char *str, int was_quoted);
t_token_info			extract_token_with_quote_info(const char **s);
void					execute_nodes(ASTNode **head, t_ast **env);
int						is_special_char(char c);
void					exec_pipe_right(ASTNode *node, t_ast **env,
							int output_fd, int *fd);
void					exec_pipe_left(ASTNode *node, t_ast **env, int input_fd,
							int *fd);
void					exec_command_node(ASTNode *node, t_ast **env,
							int input_fd, int output_fd);
void					exec_pipe_node(ASTNode *node, t_ast **env, int input_fd,
							int output_fd);
void					exec_ast(ASTNode *node, t_ast **env, int input_fd,
							int output_fd);
void					exec_cmd(ASTNode *node, t_ast **env, int child);
int						apply_redirections(ASTNode *node);
void					unlink_redirection(t_token **lst);

// Pour les test
void					pwd_recognition(t_ast **env);
void					env_recognition(char **tab, int j, t_ast **env);
void					echo_recognition(char **argv, int i, t_ast **env);
void					cd_recognition(char **argv, int i, t_ast **env);
void					build_in(char **argv, int i, t_ast **env);
void					export_recognition(char **argv, int i, t_ast **env);
void					add_export(char *argv, t_ast **env);
void					show_env(t_ast **env);
void					unset_env(char *argv, t_ast **env);
void					unset_recognition(char **argv, int i, t_ast **env);
void					show_export(t_ast **env);
char					*cat_dup(char *s1);
int						parse_exp(char *argv);
int						check_dbl_equal(char *argv);
int						skip_isspace(char *argv);
void					add_env(char *argv, t_ast **env);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					unset_exp(char *argv, t_ast **env);
void					redirection(char **argv, int i);
void					output_recognition(char **argv, int i);
void					initialise_env(t_ast **env, char **envp);
void					initialise_exp(t_ast **env, char **envp);
int						cmd(char **tab, char *path, t_ast **env);
int						cmd_recognize(char *tab);
void					free_split(char **split);
void					pwd_change(char *pwd, char *oldpwd, t_ast **env);
void					initialise_shlvl(t_ast **env);
char					*number_shlvl(t_ast **env);
int						ft_atoi(const char *nptr);
char					*ft_itoa(int n);
void					print_error(int num, char *tab, t_ast **env);
int						access_error(char *tab);
void					free_ast_tree(t_ast *node);
int						ft_isdigit(int i);
int						search_value(char *str, t_ast **env);
void					exit_recognition(char **argv, int i, t_ast **env);
void					free_env_complete(t_ast *env);
void					valid_number_fail(t_ast **env, char *arg);
void					num_has_sign(t_ast **env);
void					free_both(char *target, t_ast *current);
int						tab_len(t_ast *current);
void					cd_exit_code(void);
char					*path_var_set(t_ast *env, const char *key);
char					*full_path(char **paths, const char *cmd);
void					unset_exp_fnc(t_ast *current, char *target, char **temp,
							int j);
void					del_export(t_ast **env);
void					unset_env_fnc(t_ast *current, char *argv, char **temp,
							int j);
void					add_env_fnc(t_ast *current, char **temp, char *argv);
void					add_exp_fnc(t_ast *current, char **temp, char *argv);
char					*get_env_var(t_ast **env, char *str);
void					free_buffer(char *buffer, char *buffer2, t_ast **env);
void					cd_rec_fnc(char *tab, char *buffer, char *buffer2,
							t_ast **env);
void					free_tab1(char *buffer, char *buffer2);
void					cd_only(char **tab, int i, t_ast **env);

// gnl
char					*ft_strjoin(char const *s1, char const *s2);
int						ft_newline(const char *str);
char					*ft_replace(char *buffer);
char					*ft_extract(char *buffer);
char					*ft_buffer(int fd, char *buffer);
char					*get_next_line(int fd);

// Heredoc
void					clean_heredoc(char **argv);
void					read_heredoc(char *limiter, int quoted_limiter,
							t_ast **env);
void					free_tab(char **tab);
void					check_heredoc(t_token **lst, t_ast **env);
void					copy_tmp_to_file(const char *temp_path,
							const char *filename);
void					start_heredoc(char *limiter, int quoted_limiter,
							t_ast **env);

void					setup_sigint_handler(void);
void					setup_sigquit_handler(void);
void					disable_echoctl(void);
void					setup_heredoc_signals(void);
void					handle_sigint_heredoc(int sig_num);
void					restore_parent_signals(void);
char					**split_quote_aware(const char *s, const char *delims);
char					**split_bash_style(const char *input);
t_token_info			*split_bash_style_with_quotes(const char *input,
							int *token_count);
void					exit_status(t_token **lst, t_ast **env);
int						check_syntax_errors(t_token *lst);
void					ft_putstr_fd(char *s, int fd);
void					print_exit_code(t_ast **env);
void					handle_errno_error(const char *path);
void					command_not_found_error(const char *cmd);
void					exit_child(int exit_code, int child);
ASTNode					*parse_pipeline(t_token **lst_ptr, t_ast **env);
void					free_tokens(char **tokens);
void					free_token_info_array(t_token_info *tokens, int count);

#endif