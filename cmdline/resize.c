/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 15:20:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 01:46:35 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

extern t_cmdline	*g_cmdline;

void				handle_sigwinch(int sig)
{
	t_cmdline	*cmdline;

	(void)sig;
	cmdline = g_cmdline;
	go_to_offset(cmdline, 0);
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	write(STDOUT_FILENO, cmdline->prompt.str, ft_strlen(cmdline->prompt.str));
	update_winsize(cmdline);
	set_cursor_pos(&cmdline->cursor);
	cmdline->prompt.offset = cmdline->cursor.x;
	print_cmdline(cmdline);
}
