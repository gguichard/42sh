/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 10:03:17 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 21:08:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "shell.h"
#include "input.h"

extern t_shell	*g_shell;

static void		clean_exit(int sig)
{
	reset_term(g_shell);
	ft_putendl_fd("exit", 2);
	exit(sig);
}

void			kill_forked_pids(t_shell *shell, int sig)
{
	t_list	*curr;

	curr = shell->fork_pids;
	while (curr != NULL)
	{
		kill(*((pid_t *)curr->content), sig);
		curr = curr->next;
	}
}

static void		handle_signal(int sig)
{
	if (sig == SIGWINCH)
		update_winsize(&(g_shell->term));
	else if (sig == SIGQUIT)
		kill_forked_pids(g_shell, SIGINT);
	else if (sig == SIGINT)
	{
		if (g_shell->fork_pids != NULL)
		{
			kill_forked_pids(g_shell, SIGINT);
			ft_putchar_fd('\n', 2);
		}
		else
		{
			if (g_shell->term.visual_mode)
				vm_toggle(g_shell, &(g_shell->term));
			else
			{
				move_cursor_end(g_shell, &(g_shell->term));
				ft_putchar_fd('\n', 2);
				reset_cmdline(g_shell, g_shell->term.prompt);
			}
		}
	}
}

void			setup_signals(void)
{
	signal(SIGHUP, clean_exit);
	signal(SIGTERM, clean_exit);
	signal(SIGINT, clean_exit);
	signal(SIGCONT, clean_exit);
	signal(SIGQUIT, handle_signal);
	signal(SIGWINCH, handle_signal);
	signal(SIGINT, handle_signal);
	signal(SIGTSTP, handle_signal);
}
