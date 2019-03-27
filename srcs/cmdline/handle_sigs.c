/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 15:58:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/27 01:14:54 by gguichar         ###   ########.fr       */
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
	if (cmdline->visual.toggle)
		handle_toggle_visual(cmdline);
	// write(STDOUT_FILENO, "^C", 2);
	handle_end_key(cmdline);
	write(STDOUT_FILENO, "\n", 1);
	reset_cmdline(cmdline, get_prompt(cmdline, PROMPT_DEFAULT));
	// TODO: set last ret to 1
}

void				handle_sigwinch(int sig)
{
	t_cmdline	*cmdline;

	(void)sig;
	cmdline = g_cmdline;
	go_to_offset(cmdline, 0);
	update_winsize(cmdline);
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("cd", NULL), cmdline->winsize.ws_row - cmdline->cursor.y
			, t_putchar);
	print_prompt_and_cmdline(cmdline);
}
