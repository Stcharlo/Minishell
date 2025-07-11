/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:58:48 by agaroux           #+#    #+#             */
/*   Updated: 2025/07/11 15:49:26 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_exit_code;

void process(int sig_num)
{
    if (!kill(g_pid, sig_num))
    {
        if (sig_num == SIGQUIT)
        {
        ft_putstr_fd("Quit: 3\n", 1);
        g_exit_code = 131;
        }
        else if (sig_num == SIGINT)
        {
            write(1, "\n", 1);
            g_exit_code = 130;
        }
    }
    else if (sig_num == SIGINT)
    {
        write(1, "\n", 1);
        g_exit_code = 1;
    }
}

void        sigint_handler(int sig_num, pid_t g_pid)
{
    if ((sig_num == SIGINT || sig_num == SIGQUIT) && g_pid != 0)
        process(sig_num);
    else
    {
        if (sig_num == SIGINT)
        {
            write(1, "\n", 1);
            g_exit_code = 1;
        }
        else if (sig_num == SIGQUIT)
        {
            write(1, "\b\b \b\b", 5);
            g_exit_code = 2;
        }
    }
}