/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_moves.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 11:15:36 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/12 16:22:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_move_left(t_cmdline *cmdline)
{
	if (cmdline->input.offset == 0)
		return (0);
	if (cmdline->cursor.x == 1)
	{
		tputs(tgetstr("up", NULL), 1, t_putchar);
		tputs(tparm(tgetstr("RI", NULL), cmdline->winsize.ws_col), 1
				, t_putchar);
		set_cursor_pos(&cmdline->cursor);
	}
	else
	{
		tputs(tgetstr("le", NULL), 1, t_putchar);
		cmdline->cursor.x -= 1;
	}
	cmdline->input.offset -= 1;
	return (1);
}

int	handle_move_right(t_cmdline *cmdline)
{
	if (cmdline->input.offset == cmdline->input.size)
		return (0);
	if (cmdline->cursor.x == cmdline->winsize.ws_col)
	{
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("do", NULL), 1, t_putchar);
		set_cursor_pos(&cmdline->cursor);
	}
	else
	{
		tputs(tgetstr("nd", NULL), 1, t_putchar);
		cmdline->cursor.x += 1;
	}
	cmdline->input.offset += 1;
	return (1);
}
