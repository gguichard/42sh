/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 15:58:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 16:23:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <signal.h>
#include <term.h>
#include "shell.h"
#include "cmdline.h"

void				handle_sig(int sig)
{
	t_cmdline	*cmdline;

	(void)sig;
	cmdline = g_cmdline;
	if (g_in_exec)
	{
		kill(g_pid, SIGINT);
		g_in_exec = 0;
		g_pid = -1;
	}
	else
	{
		write(STDOUT_FILENO, "^C", 2);
		handle_end_key(cmdline);
		write(STDOUT_FILENO, "\n", 1);
		reset_cmdline(cmdline, get_prompt(cmdline, PROMPT_DEFAULT));
	}
	// TODO: set last ret to 1
}

void				handle_sigwinch(int sig)
{
	t_cmdline	*cmdline;

	(void)sig;
	cmdline = g_cmdline;
	go_to_offset(cmdline, 0);
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("cd", NULL), cmdline->winsize.ws_row - cmdline->cursor.y
			, t_putchar);
	write(STDOUT_FILENO, cmdline->prompt.str, ft_strlen(cmdline->prompt.str));
	update_winsize(cmdline);
	set_cursor_pos(&cmdline->cursor);
	cmdline->prompt.offset = cmdline->cursor.x;
	print_cmdline(cmdline);
}
