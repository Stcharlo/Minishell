/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:47:21 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/17 11:10:22 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_exit_code = 0;

/// @brief keeps on reading user input, add_history of input,
///	checks for cmds such as clear and exit and calls process_tokens
/// @param lst chained list for the tokenisation
/// @param env
void	infinite_read(t_token **lst, t_ast **env)
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
			continue ;
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

/// @brief reads a complete line of the user input and 
/// checks if there are open quotes once quotes are 
/// closed it sends the user input
/// @param
/// @return unparsed input from the user
char	*get_input(void)
{
	char	*line;
	char	*tmp;

	line = readline("Minishell> ");
	if (line == NULL)
		return (NULL);
	while (open_quotes(line))
	{
		tmp = readline_open_quotes(line);
		if (!tmp)
		{
			free(line);
			return (NULL);
		}
		free(line);
		line = tmp;
	}
	return (line);
}

void	unlink_redirection(t_token **lst)
{
	t_token	*tmp;

	tmp = *lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->value, "<<"))
			unlink(tmp->next->value);
		tmp = tmp->next;
	}
}

/// @brief main function calling infinite read
/// @param argc
/// @param argv
/// @param env
/// @return
int	main(int argc, char **argv, char **env)
{
	int		final_exit_code;
	t_token	*list;
	t_token	**lst;
	t_ast	**ast_head;
	t_ast	*ast;

	(void)argc;
	(void)argv;
	ast = NULL;
	ast_head = &ast;
	list = NULL;
	lst = &list;
	setup_sigint_handler();
	setup_sigquit_handler();
	disable_echoctl();
	initialise_env(ast_head, env);
	initialise_exp(ast_head, env);
	initialise_shlvl(ast_head);
	infinite_read(lst, ast_head);
	rl_clear_history();
	final_exit_code = ast->env->error_code;
	free_env_complete(ast);
	if (g_exit_code >= 128)
		return (g_exit_code);
	return (final_exit_code % 256);
}
