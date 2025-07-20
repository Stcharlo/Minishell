/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stcharlo <stcharlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:28:11 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/20 19:20:14 by stcharlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_code;

void	handle_sigint_parent(int sig_num)
{
	write(1, "Minishell> \n", 12);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_code = 128 + sig_num;
}

void	handle_sigquit_parent(int sig_num)
{
	(void)sig_num;
	rl_on_new_line();
	rl_redisplay();
	g_exit_code = 128 + sig_num;
}

void	setup_sigint_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint_parent;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

void	setup_sigquit_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigquit_parent;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
}

void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}
