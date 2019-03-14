/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_moves.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 11:15:36 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 12:43:07 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_move_left(t_cmdline *cmdline)
{
	cmdline->saved_col = -1;
	if (cmdline->input.offset == 0)
		return (0);
	cmdline->input.offset -= 1;
	if (cmdline->cursor.x > 0)
	{
		tputs(tgetstr("le", NULL), 1, t_putchar);
		cmdline->cursor.x -= 1;
	}
	else
	{
		cmdline->row -= 1;
		cmdline->cursor.y -= 1;
		cmdline->cursor.x = get_rightmost_column(cmdline
				, cmdline->input.offset);
		go_to_cursor_pos(cmdline->cursor);
	}
	return (1);
}

int	handle_move_right(t_cmdline *cmdline)
{
	cmdline->saved_col = -1;
	if (cmdline->input.offset == cmdline->input.size)
		return (0);
	cmdline->cursor.x += 1;
	if (cmdline->cursor.x < cmdline->winsize.ws_col
			&& cmdline->input.buffer[cmdline->input.offset] != '\n')
		tputs(tgetstr("nd", NULL), 1, t_putchar);
	else
	{
		cmdline->row += 1;
		cmdline->cursor.y += 1;
		cmdline->cursor.x = 0;
		go_to_cursor_pos(cmdline->cursor);
	}
	cmdline->input.offset += 1;
	return (1);
}
