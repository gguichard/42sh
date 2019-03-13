/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_moves.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 11:15:36 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 11:43:18 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

static int	get_leftmost_column(t_cmdline *cmdline)
{
	int	idx;
	int	col;

	idx = cmdline->input.offset - 1;
	if (cmdline->input.buffer[idx] != '\n')
		return (cmdline->winsize.ws_col);
	else
	{
		col = 0;
		while (idx > 0 && cmdline->input.buffer[--idx] != '\n')
			col++;
		if (idx == 0)
			col += cmdline->prompt.offset;
		if (cmdline->winsize.ws_col <= 0)
			return (1);
		else
			return (col % cmdline->winsize.ws_col + 1);
	}
}

int			handle_move_left(t_cmdline *cmdline)
{
	if (cmdline->input.offset == 0)
		return (0);
	if (cmdline->cursor.x != 1)
	{
		tputs(tgetstr("le", NULL), 1, t_putchar);
		cmdline->cursor.x -= 1;
	}
	else
	{
		cmdline->row -= 1;
		cmdline->cursor.y -= 1;
		cmdline->cursor.x = get_leftmost_column(cmdline);
		go_to_cursor_pos(cmdline->cursor);
	}
	cmdline->input.offset -= 1;
	return (1);
}

int			handle_move_right(t_cmdline *cmdline)
{
	if (cmdline->input.offset == cmdline->input.size)
		return (0);
	if (cmdline->cursor.x != cmdline->winsize.ws_col
			&& cmdline->input.buffer[cmdline->input.offset] != '\n')
	{
		tputs(tgetstr("nd", NULL), 1, t_putchar);
		cmdline->cursor.x += 1;
	}
	else
	{
		cmdline->row += 1;
		cmdline->cursor.y += 1;
		cmdline->cursor.x = 1;
		go_to_cursor_pos(cmdline->cursor);
	}
	cmdline->input.offset += 1;
	return (1);
}
